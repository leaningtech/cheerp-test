#!/usr/bin/env python3
"""Determinism Testing Wrapper (print-after based) for Cheerp LIT tests.

This wrapper is intentionally different from helpers/determinism_wrapper.py:
- It does NOT compare final output artifacts.
- Instead it uses the clang driver '-###' output to re-run the underlying
  pipeline commands and injects LLVM '-print-after' (or '-print-after-all')
  into the optimizer/codegen steps, then hashes the resulting IR dumps.

The intent is to mirror the approach used by cheerp-utils/tests/run_tests.py.

Notes / limitations:
- This wrapper only understands a subset of LIT substitutions. It scans the
  test file for '// RUN:' lines containing compile_mode_<target> and extracts
  the compile command portion (up to '&&' or '|').
- It uses its own implementation of compile_mode_<target> (based on the same
  defaults as lit.cfg) rather than invoking lit to expand substitutions.

Typical usage from run_lit_tests.py:
  python3 helpers/determinism_wrapper_print_after.py --suite . --target js,wasm
"""

from __future__ import annotations

import argparse
import concurrent.futures
import hashlib
import os
import random
import re
import shlex
import subprocess
import sys
import tempfile
from dataclasses import dataclass
from pathlib import Path


CHEERP_PASSES: list[str] = [
    "StructMemFuncLowering",
    "FreeAndDeleteRemoval",
    "GlobalDepsAnalyzer",
    "FixIrreducibleControlFlow",
    "IdenticalCodeFolding",
    "GEPOptimizer",
    "Registerize",
    "PointerAnalyzer",
    "DelayInsts",
    "AllocaMerging",
    "AllocaArrays",
    "AllocaStoresExtractor",
    "TypeOptimizer",
    "PreExecute",
    "ExpandStructRegs",
    "CheerpLowerSwitch",
]


def _md5_hex(data: bytes) -> str:
    return hashlib.md5(data).hexdigest()


def _stable_seed_int(s: str) -> int:
    return int(_md5_hex(s.encode("utf-8")), 16)


def _clamp01(x: float) -> float:
    try:
        x = float(x)
    except Exception:
        return 0.0
    return max(0.0, min(1.0, x))


def should_test_determinism(probability: float) -> bool:
    return random.random() < probability    


def select_print_after_flags(*, level: int, seed: int) -> str:
    """Return a shell-appendable string of -print-after flags.

    Mirrors cheerp-utils/tests/run_tests.py semantics:
    - level == -1: test all passes via '-print-after-all'
    - level == 0: no determinism
    - level == 1: no '-print-after' injection (not very useful here)
    - level >= 2: inject (level-1) passes via '-print-after=<pass>'
    """

    if level == -1:
        return " -print-after-all"
    if level <= 1:
        return ""

    num = level - 1
    res = ""
    for i in range(num):
        res += " -print-after=" + CHEERP_PASSES[(seed + i) % len(CHEERP_PASSES)]
    return res


def _normalize_opt_level(opt_level: str) -> str:
    opt_level = (opt_level or "-O1").strip()
    if not opt_level.startswith("-"):
        if not opt_level.startswith("O"):
            opt_level = "-O" + opt_level
        else:
            opt_level = "-" + opt_level
    return opt_level


def _split_flags(flags: str) -> list[str]:
    flags = (flags or "").strip()
    if not flags:
        return []
    return shlex.split(flags)


def _tests_root_from_this_file() -> Path:
    # .../tests/helpers/determinism_wrapper_print_after.py -> .../tests
    return Path(__file__).resolve().parents[1]


@dataclass(frozen=True)
class CompileInvocation:
    test_file: Path
    target: str  # js|wasm|asmjs
    argv: list[str]  # full argv including -o and source path


def base_compile_mode_argv(
    *,
    compiler: str,
    target: str,
    opt_level: str,
    include_dir: Path,
    cheerp_flags: str,
    asan: bool,
) -> list[str]:
    argv: list[str] = [compiler]
    argv += _split_flags(cheerp_flags)
    argv += [
        _normalize_opt_level(opt_level),
        "-frtti",
        "-cheerp-bounds-check",
        "-cheerp-fix-wrong-func-casts",
        f"-I{str(include_dir)}",
    ]

    if target == "wasm":
        argv += ["-target", "cheerp-wasm"]
        if asan:
            argv += ["-fsanitize=address"]
    elif target == "asmjs":
        argv += ["-target", "cheerp-wasm", "-cheerp-linear-output=asmjs"]
        if asan:
            argv += ["-fsanitize=address"]
    elif target == "js":
        argv += ["-target", "cheerp"]
        # ASAN never applies to JS
    else:
        raise ValueError(f"Unknown target: {target}")

    return argv


_RUN_LINE_RE = re.compile(r"^\s*//\s*RUN:\s*(.*)$")
_XFAIL_RE = re.compile(r"^\s*//\s*XFAIL:\s*(.*)$")
_REQUIRES_RE = re.compile(r"^\s*//\s*REQUIRES:\s*(.*)$")


def iter_run_lines(test_file: Path) -> list[str]:
    lines: list[str] = []
    with test_file.open("r", encoding="utf-8", errors="replace") as f:
        for raw in f:
            m = _RUN_LINE_RE.match(raw)
            if not m:
                continue
            lines.append(m.group(1).strip())
    return lines


def has_xfail(test_file: Path) -> bool:
    with test_file.open("r", encoding="utf-8", errors="replace") as f:
        for raw in f:
            if _XFAIL_RE.match(raw):
                return True
    return False


def required_features(test_file: Path) -> set[str]:
    feats: set[str] = set()
    with test_file.open("r", encoding="utf-8", errors="replace") as f:
        for raw in f:
            m = _REQUIRES_RE.match(raw)
            if not m:
                continue
            # LIT syntax typically uses comma-separated feature names
            for token in m.group(1).replace(",", " ").split():
                token = token.strip()
                if token:
                    feats.add(token)
    return feats


def _extract_compile_segment(run_line: str, token: str) -> str | None:
    idx = run_line.find(token)
    if idx < 0:
        return None

    seg = run_line[idx:]
    # Keep only the compilation part, drop '&& node ...' / '| FileCheck ...'
    cut_positions = [p for p in (seg.find("&&"), seg.find("|")) if p != -1]
    if cut_positions:
        seg = seg[: min(cut_positions)]
    return seg.strip()


def _filter_redirections(argv: list[str]) -> list[str]:
    """Drop common shell redirections that appear in RUN lines (2>&1, >/dev/null, etc.)."""
    filtered: list[str] = []
    skip_next = False
    for a in argv:
        if skip_next:
            skip_next = False
            continue

        if a in ("2>&1", "1>&2", "1>&1"):
            continue
        if a in (">", "1>", "2>", ">>", "1>>", "2>>"):
            skip_next = True
            continue
        if a.startswith("2>") or a.startswith("1>"):
            # e.g. 2>/dev/null
            continue
        filtered.append(a)
    return filtered


def compile_invocations_for_test(
    *,
    test_file: Path,
    targets: set[str],
    compiler: str,
    opt_level: str,
    cheerp_flags: str,
    asan: bool,
    temp_dir: Path,
) -> list[CompileInvocation]:
    run_lines = iter_run_lines(test_file)
    include_dir = _tests_root_from_this_file()

    invocations: list[CompileInvocation] = []

    for target in sorted(targets):
        token = f"compile_mode_{target}"
        # Prefer regular_only lines (avoid preexec_only variants)
        candidates: list[str] = []
        for rl in run_lines:
            if "preexec_only" in rl:
                continue
            seg = _extract_compile_segment(rl, token)
            if seg is not None:
                candidates.append(seg)

        # Fallback: accept any line that contains the token
        if not candidates:
            for rl in run_lines:
                seg = _extract_compile_segment(rl, token)
                if seg is not None:
                    candidates.append(seg)

        if not candidates:
            continue

        # Use the first matching compile segment
        seg = candidates[0]
        # Drop the leading token and keep the extra args part
        extra = seg[len(token) :].strip()

        # Substitute (a subset of) LIT placeholders.
        # Important: do this in an order that does not accidentally rewrite other
        # substitution names. For example, %then_run_js starts with "%t".
        extra = re.sub(r"%then_run_(js|wasm|asmjs)\b", "", extra)

        abs_test = str(test_file.resolve())
        extra = re.sub(r"%s\b", abs_test, extra)
        extra = re.sub(r"%t\b", str(temp_dir), extra)

        extra_argv = _filter_redirections(shlex.split(extra))

        base_argv = base_compile_mode_argv(
            compiler=compiler,
            target=target,
            opt_level=opt_level,
            include_dir=include_dir,
            cheerp_flags=cheerp_flags,
            asan=asan,
        )

        full_argv = base_argv + extra_argv
        invocations.append(CompileInvocation(test_file=test_file, target=target, argv=full_argv))

    return invocations


def _extract_pipeline_commands_from_clang_dash_hash_hash_hash(stderr_text: str) -> list[str]:
    """Parse 'clang -###' stderr and return a list of shell command lines."""
    lines = stderr_text.splitlines()
    lines_to_execute: list[str] = []

    last_lines = False
    for line in lines:
        if last_lines and len(line) > 1 and line.strip() != "(in-process)":
            lines_to_execute.append(line)
        if "InstalledDir" in line[:14]:
            last_lines = True

    # If parsing failed (format differs), fall back to collecting quoted command lines.
    if not lines_to_execute:
        for line in lines:
            if line.startswith(" ") and '"' in line:
                lines_to_execute.append(line)

    return lines_to_execute


def _tail_lines(text: str, max_lines: int = 120) -> str:
    lines = (text or "").splitlines()
    if len(lines) <= max_lines:
        return "\n".join(lines)
    return "\n".join(lines[-max_lines:])


def produce_print_after_report(
    *,
    compile_argv: list[str],
    print_after_flags: str,
    print_cmd: bool,
) -> str:
    """Return a normalized string report suitable for hashing/comparison."""

    cmd = compile_argv + ["-save-temps=obj", "-###"]
    if print_cmd:
        print("Command (driver):", " ".join(shlex.quote(a) for a in cmd))

    res = subprocess.run(cmd, stderr=subprocess.PIPE, stdout=subprocess.DEVNULL, text=True)
    if res.returncode != 0:
        raise RuntimeError(
            "clang -### failed "
            f"(rc={res.returncode})\n"
            f"cmd={' '.join(shlex.quote(a) for a in cmd)}\n"
            f"stderr(tail)=\n{_tail_lines(res.stderr)}"
        )

    lines_to_execute = _extract_pipeline_commands_from_clang_dash_hash_hash_hash(res.stderr)
    if not lines_to_execute:
        raise RuntimeError("Failed to extract pipeline commands from clang -### output")

    output_lines: list[str] = []
    for i, line in enumerate(lines_to_execute):
        cmdline = line
        # Heuristic inherited from run_tests.py: first few commands are driver/llvm-link;
        # inject -print-after only for later optimizer/codegen steps.
        if i >= 4 and print_after_flags:
            cmdline += print_after_flags

        if print_cmd:
            print("Command (pipe):", cmdline)

        pipe_res = subprocess.run(
            cmdline,
            shell=True,
            stderr=subprocess.PIPE,
            stdout=subprocess.DEVNULL,
            text=True,
        )
        if pipe_res.returncode != 0:
            raise RuntimeError(
                "pipeline command failed "
                f"(rc={pipe_res.returncode})\n"
                f"cmd={cmdline}\n"
                f"stderr(tail)=\n{_tail_lines(pipe_res.stderr)}"
            )

        if pipe_res.stderr:
            output_lines.extend(pipe_res.stderr.splitlines(True))

    # Normalize away temp ModuleID paths (often unstable)
    normalized: list[str] = []
    for line in output_lines:
        idx = line.find("ModuleID")
        if idx != -1:
            normalized.append(line[:idx] + "\n")
        else:
            normalized.append(line)

    return "".join(normalized)


def _safe_filename(s: str) -> str:
    return re.sub(r"[^A-Za-z0-9._-]+", "_", s)


def check_invocation_determinism(
    *,
    inv: CompileInvocation,
    level: int,
    runs: int,
    probability: float,
    print_cmd: bool,
    failures_dir: Path,
) -> tuple[bool, str]:
    label = f"{inv.test_file.name}:{inv.target}"

    if level == 0:
        return True, f"SKIP (determinism disabled): {label}"

    if not should_test_determinism(probability):
        return True, f"SKIP (probability): {label}"

    if runs <= 0:
        runs = 1

    seed = _stable_seed_int(str(inv.test_file.resolve()) + ":" + inv.target)
    print_after_flags = select_print_after_flags(level=level, seed=seed)

    # Note: for level==1, print_after_flags is empty, which typically yields an empty report.
    baseline_report: str | None = None
    baseline_hash: str | None = None

    for r in range(runs):
        try:
            report = produce_print_after_report(
                compile_argv=inv.argv,
                print_after_flags=print_after_flags,
                print_cmd=print_cmd,
            )
        except Exception as e:
            return False, f"ERROR (print-after): {label}\n  {e}"

        h = _md5_hex(report.encode("utf-8", errors="replace"))

        if r == 0:
            baseline_report = report
            baseline_hash = h
            continue

        if h != baseline_hash:
            failures_dir.mkdir(parents=True, exist_ok=True)
            base = _safe_filename(f"{inv.test_file.name}.{inv.target}.level{level}")
            out0 = failures_dir / f"{base}.run0.txt"
            outn = failures_dir / f"{base}.run{r}.txt"
            out0.write_text(baseline_report or "", encoding="utf-8", errors="replace")
            outn.write_text(report, encoding="utf-8", errors="replace")

            msg = (
                f"DETERMINISM FAILURE (print-after): {label}\n"
                f"  run0: {baseline_hash}\n"
                f"  run{r}: {h}\n"
                f"  saved:\n    {out0}\n    {outn}"
            )
            return False, msg

    return True, f"DETERMINISM PASS (print-after): {label}"


def find_test_files(root: Path, *, exclude_dirs: set[str]) -> list[Path]:
    tests: list[Path] = []
    for ext in ("*.cpp", "*.c"):
        tests.extend(root.rglob(ext))

    # Exclude Output and failure dirs + user-requested directories
    filtered: list[Path] = []
    for p in tests:
        parts = set(p.parts)
        if "Output" in parts or "determinism_failures" in parts:
            continue
        if any(ex in parts for ex in exclude_dirs):
            continue
        filtered.append(p)

    return sorted(set(filtered))


def main() -> int:
    ap = argparse.ArgumentParser(description="Cheerp LIT determinism (print-after based)")
    ap.add_argument("test_file", nargs="?", help="Single test file to check")
    ap.add_argument("--suite", help="Directory containing test suite")
    ap.add_argument("--level", type=int, default=2, help="Determinism level (run_tests.py semantics). 0=off, 2+=pass dumps, -1=all")
    ap.add_argument("--runs", type=int, default=3, help="Number of report runs to compare")
    ap.add_argument("--probability", type=float, default=1.0, help="Probability of checking each (test,target) invocation")
    ap.add_argument("--jobs", type=int, default=1, help="Parallel jobs")
    ap.add_argument("--target", default="js,wasm,asmjs", help="Targets: js,wasm,asmjs (comma-separated)")
    ap.add_argument("--compiler", default="/opt/cheerp/bin/clang++", help="Compiler driver to run")
    ap.add_argument("--opt-level", default="-O1", help="Optimization level (e.g. -O1)")
    ap.add_argument("--cheerp-flags", default="", help="Extra compiler flags")
    ap.add_argument("--asan", action="store_true", help="Enable ASAN for wasm/asmjs targets (adds -fsanitize=address)")
    ap.add_argument("--print-cmd", action="store_true", help="Print commands being executed")
    ap.add_argument("--keep-temps", action="store_true", help="Keep temporary directories used for %%t expansion")
    ap.add_argument(
        "--exclude-dir",
        action="append",
        default=[],
        help="Directory name to exclude from --suite discovery (repeatable, e.g. --exclude-dir=threading)",
    )
    ap.add_argument(
        "--include-xfail",
        action="store_true",
        help="Include tests marked with // XFAIL: (default: skip them)",
    )

    args = ap.parse_args()

    if not args.test_file and not args.suite:
        ap.error("Must specify either test_file or --suite")
    if args.test_file and args.suite:
        ap.error("Cannot specify both test_file and --suite")

    probability = _clamp01(args.probability)

    targets: set[str] = set()
    for t in (args.target or "").split(","):
        t = t.strip().lower()
        if not t:
            continue
        if t not in ("js", "wasm", "asmjs"):
            ap.error(f"Unknown target '{t}'")
        targets.add(t)
    if not targets:
        targets = {"js", "wasm", "asmjs"}

    exclude_dirs = {str(d).strip() for d in (args.exclude_dir or []) if str(d).strip()}

    if args.test_file:
        test_files = [Path(args.test_file)]
        # Still honor exclude_dirs if the single file is under an excluded path
        if any(ex in set(test_files[0].parts) for ex in exclude_dirs):
            print(f"Skipping excluded test file: {test_files[0]}")
            return 0
    else:
        test_files = find_test_files(Path(args.suite), exclude_dirs=exclude_dirs)

    if not test_files:
        print("No test files found")
        return 1

    jobs = max(1, int(args.jobs))
    failures_dir = _tests_root_from_this_file() / "determinism_failures_print_after"

    # Ensure Cheerp tools can be found when clang -### emits bare tool names.
    compiler_dir = str(Path(args.compiler).resolve().parent)
    os.environ["PATH"] = compiler_dir + os.pathsep + os.environ.get("PATH", "")

    # Precompute all invocations (each test x each target) and create a temp dir for %t.
    tmp_root_ctx = tempfile.TemporaryDirectory(prefix="lit_det_print_after_")
    tmp_root = Path(tmp_root_ctx.name)

    # Approximate LIT feature availability (minimal; enough for common REQUIRES like 'wasm')
    available_features: set[str] = set(targets)
    if "wasm" in targets or "asmjs" in targets:
        available_features.add("linear-memory")
    if "wasm" in targets or "js" in targets:
        available_features.add("packed_tests")
    if args.asan:
        available_features.add("asan")
    else:
        available_features.add("no-asan")

    tasks: list[CompileInvocation] = []
    for tf in test_files:
        if (not args.include_xfail) and has_xfail(tf):
            continue

        reqs = required_features(tf)
        if reqs and not reqs.issubset(available_features):
            # Skip tests that require features we are not currently checking
            continue

        tag = _md5_hex(str(tf.resolve()).encode("utf-8"))[:8]
        tmpdir = tmp_root / f"{tf.stem}_{tag}"
        tmpdir.mkdir(parents=True, exist_ok=True)

        invs = compile_invocations_for_test(
            test_file=tf,
            targets=targets,
            compiler=args.compiler,
            opt_level=args.opt_level,
            cheerp_flags=args.cheerp_flags,
            asan=args.asan,
            temp_dir=tmpdir,
        )
        tasks.extend(invs)

    if not tasks:
        print("No compile_mode_<target> RUN lines found in selected tests")
        return 1

    total = len(tasks)
    passed = 0
    failed = 0
    skipped = 0

    print(f"Running {total} compile invocations with print-after determinism")
    print(f"  level={args.level} runs={args.runs} probability={probability} jobs={jobs}")
    print(f"  targets={','.join(sorted(targets))}")
    if exclude_dirs:
        print(f"  exclude_dirs={','.join(sorted(exclude_dirs))}")

    def _run_one(inv: CompileInvocation) -> tuple[bool, str]:
        return check_invocation_determinism(
            inv=inv,
            level=args.level,
            runs=args.runs,
            probability=probability,
            print_cmd=args.print_cmd,
            failures_dir=failures_dir,
        )

    try:
        with concurrent.futures.ThreadPoolExecutor(max_workers=jobs) as ex:
            futures = [ex.submit(_run_one, inv) for inv in tasks]
            for inv, fut in zip(tasks, futures):
                ok, msg = fut.result()
                label = f"{inv.test_file.name}:{inv.target}"
                if msg.startswith("SKIP"):
                    skipped += 1
                    print(f"[skip] {label}")
                    continue
                if ok:
                    passed += 1
                    print(f"[pass] {label}")
                else:
                    failed += 1
                    print(f"[fail] {label}\n{msg}")

    finally:
        if not args.keep_temps:
            tmp_root_ctx.cleanup()
        else:
            print(f"Temporary dirs kept under: {tmp_root}")

    print("=" * 60)
    print("Summary")
    print(f"  total:   {total}")
    print(f"  passed:  {passed}")
    print(f"  failed:  {failed}")
    print(f"  skipped: {skipped}")
    if failed == 0:
        return 0
    return 1


if __name__ == "__main__":
    sys.exit(main())
