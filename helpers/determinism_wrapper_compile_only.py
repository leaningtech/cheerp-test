#!/usr/bin/env python3
"""Compile-only determinism wrapper for Cheerp LIT tests.

This wrapper intentionally avoids invoking `lit` directly for each test.
It extracts `compile_mode_<target>` commands from `// RUN:` lines, compiles
multiple times, and compares generated artifacts by hash.
"""

from __future__ import annotations

import argparse
import concurrent.futures
import hashlib
import os
import re
import shlex
import shutil
import subprocess
import sys
import tempfile
from dataclasses import dataclass
from pathlib import Path


_RUN_LINE_RE = re.compile(r"^\s*//\s*RUN:\s*(.*)$")
_XFAIL_RE = re.compile(r"^\s*//\s*XFAIL:\s*(.*)$")
_REQUIRES_RE = re.compile(r"^\s*//\s*REQUIRES:\s*(.*)$")


@dataclass(frozen=True)
class CompileInvocation:
    test_file: Path
    target: str
    invocation_index: int
    compile_prefix: list[str]
    extra_args_template: str


def _md5_hex(data: bytes) -> str:
    return hashlib.md5(data).hexdigest()


def _md5_file(path: Path) -> str:
    h = hashlib.md5()
    with path.open("rb") as f:
        for chunk in iter(lambda: f.read(1024 * 1024), b""):
            h.update(chunk)
    return h.hexdigest()


def _md5_bytes(data: bytes) -> str:
    return hashlib.md5(data).hexdigest()


def _read_uleb(data: bytes, i: int) -> tuple[int, int]:
    result = 0
    shift = 0
    while True:
        if i >= len(data):
            raise ValueError("ULEB decode out of bounds")
        b = data[i]
        i += 1
        result |= (b & 0x7F) << shift
        if (b & 0x80) == 0:
            return result, i
        shift += 7
        if shift > 63:
            raise ValueError("ULEB decode overflow")


def _write_uleb(v: int) -> bytes:
    if v < 0:
        raise ValueError("ULEB value must be non-negative")
    out = bytearray()
    while True:
        b = v & 0x7F
        v >>= 7
        if v:
            out.append(b | 0x80)
        else:
            out.append(b)
            break
    return bytes(out)


def _read_name_bytes(data: bytes, i: int) -> tuple[bytes, int]:
    n, i = _read_uleb(data, i)
    j = i + n
    if j > len(data):
        raise ValueError("name extends past buffer")
    return data[i:j], j


def _write_name_bytes(b: bytes) -> bytes:
    return _write_uleb(len(b)) + b


def _canonicalize_import_section(payload: bytes) -> bytes:
    i = 0
    count, i = _read_uleb(payload, i)
    out = bytearray()
    out.extend(_write_uleb(count))

    for _ in range(count):
        module_name, i = _read_name_bytes(payload, i)
        _field_name, i = _read_name_bytes(payload, i)  # ignored intentionally

        if i >= len(payload):
            raise ValueError("import kind missing")
        kind = payload[i]
        i += 1

        out.extend(_write_name_bytes(module_name))
        out.extend(_write_name_bytes(b""))  # ignore symbol name for functional determinism
        out.append(kind)

        if kind == 0:  # func
            type_idx, i = _read_uleb(payload, i)
            out.extend(_write_uleb(type_idx))
        elif kind == 1:  # table
            if i >= len(payload):
                raise ValueError("table elem type missing")
            elem_type = payload[i]
            i += 1
            limits_flags, i = _read_uleb(payload, i)
            min_lim, i = _read_uleb(payload, i)
            out.append(elem_type)
            out.extend(_write_uleb(limits_flags))
            out.extend(_write_uleb(min_lim))
            if limits_flags & 0x1:
                max_lim, i = _read_uleb(payload, i)
                out.extend(_write_uleb(max_lim))
        elif kind == 2:  # memory
            limits_flags, i = _read_uleb(payload, i)
            min_lim, i = _read_uleb(payload, i)
            out.extend(_write_uleb(limits_flags))
            out.extend(_write_uleb(min_lim))
            if limits_flags & 0x1:
                max_lim, i = _read_uleb(payload, i)
                out.extend(_write_uleb(max_lim))
        elif kind == 3:  # global
            if i + 1 >= len(payload):
                raise ValueError("global type/mutability missing")
            val_type = payload[i]
            mutability = payload[i + 1]
            i += 2
            out.append(val_type)
            out.append(mutability)
        elif kind == 4:  # tag (exceptions proposal)
            if i >= len(payload):
                raise ValueError("tag attribute missing")
            attr = payload[i]
            i += 1
            type_idx, i = _read_uleb(payload, i)
            out.append(attr)
            out.extend(_write_uleb(type_idx))
        else:
            raise ValueError(f"unsupported import kind: {kind}")

    if i != len(payload):
        raise ValueError("trailing bytes in import section")
    return bytes(out)


def _canonicalize_export_section(payload: bytes) -> bytes:
    i = 0
    count, i = _read_uleb(payload, i)
    out = bytearray()
    out.extend(_write_uleb(count))

    for _ in range(count):
        _name, i = _read_name_bytes(payload, i)  # ignored intentionally
        if i >= len(payload):
            raise ValueError("export kind missing")
        kind = payload[i]
        i += 1
        idx, i = _read_uleb(payload, i)

        out.extend(_write_name_bytes(b""))  # ignore symbol name for functional determinism
        out.append(kind)
        out.extend(_write_uleb(idx))

    if i != len(payload):
        raise ValueError("trailing bytes in export section")
    return bytes(out)


def _normalize_wasm_functional(data: bytes) -> bytes:
    if len(data) < 8 or data[:4] != b"\x00asm":
        return data

    out = bytearray()
    out.extend(data[:8])  # magic + version

    i = 8
    while i < len(data):
        sec_id = data[i]
        i += 1
        sec_size, i2 = _read_uleb(data, i)
        sec_start = i2
        sec_end = sec_start + sec_size
        if sec_end > len(data):
            raise ValueError("section extends past file")
        payload = data[sec_start:sec_end]

        if sec_id == 2:
            payload = _canonicalize_import_section(payload)
        elif sec_id == 7:
            payload = _canonicalize_export_section(payload)

        out.append(sec_id)
        out.extend(_write_uleb(len(payload)))
        out.extend(payload)
        i = sec_end

    return bytes(out)


def _tail_lines(text: str, max_lines: int = 80) -> str:
    lines = (text or "").splitlines()
    if len(lines) <= max_lines:
        return "\n".join(lines)
    return "\n".join(lines[-max_lines:])


def _safe_filename(s: str) -> str:
    return re.sub(r"[^A-Za-z0-9._-]+", "_", s)


def _test_name_with_parent(test_file: Path) -> str:
    parent = test_file.parent.name
    if parent:
        return f"{parent}/{test_file.name}"
    return test_file.name


def _clamp01(x: float) -> float:
    try:
        x = float(x)
    except Exception:
        return 0.0
    return max(0.0, min(1.0, x))


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


def _repo_root_from_this_file() -> Path:
    # .../helpers/determinism_wrapper_compile_only.py -> repo root
    return Path(__file__).resolve().parents[1]


def _tests_dir_from_this_file() -> Path:
    return _repo_root_from_this_file() / "tests"


def should_test_determinism(label: str, probability: float) -> bool:
    """Deterministic probability sampling based on invocation label."""
    probability = _clamp01(probability)
    if probability <= 0.0:
        return False
    if probability >= 1.0:
        return True
    score = int(_md5_hex(label.encode("utf-8"))[:8], 16) / 0xFFFFFFFF
    return score < probability


def iter_run_lines(test_file: Path) -> list[str]:
    lines: list[str] = []
    with test_file.open("r", encoding="utf-8", errors="replace") as f:
        for raw in f:
            m = _RUN_LINE_RE.match(raw)
            if m:
                lines.append(m.group(1).strip())
    return lines


def has_xfail(test_file: Path) -> bool:
    with test_file.open("r", encoding="utf-8", errors="replace") as f:
        for raw in f:
            if _XFAIL_RE.match(raw):
                return True
    return False


def required_features(test_file: Path) -> set[str]:
    """Collect simple REQUIRES tokens.

    This is intentionally lightweight and supports the common tokenized form,
    e.g. `// REQUIRES: js, regular`.
    """
    feats: set[str] = set()
    with test_file.open("r", encoding="utf-8", errors="replace") as f:
        for raw in f:
            m = _REQUIRES_RE.match(raw)
            if not m:
                continue
            req_expr = m.group(1).replace(",", " ")
            for token in req_expr.split():
                token = token.strip().strip("()")
                if not token:
                    continue
                if token in {"&&", "||"}:
                    continue
                feats.add(token)
    return feats


def requirements_satisfied(requirements: set[str], available: set[str]) -> bool:
    if not requirements:
        return True

    for req in requirements:
        if req.startswith("!"):
            feat = req[1:]
            if feat and feat in available:
                return False
            continue
        if req not in available:
            return False
    return True


def _extract_compile_segment(run_line: str, token: str) -> str | None:
    idx = run_line.find(token)
    if idx < 0:
        return None

    seg = run_line[idx:]
    # Keep only compile portion before execution/check pipeline.
    cut_positions = [p for p in (seg.find("&&"), seg.find("|")) if p != -1]
    if cut_positions:
        seg = seg[: min(cut_positions)]
    return seg.strip()


def _filter_redirections(argv: list[str]) -> list[str]:
    """Drop common shell redirections from RUN lines."""
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
            continue
        filtered.append(a)
    return filtered


def _substitute_lit_placeholders(extra: str, test_file: Path, temp_dir: Path) -> str:
    """Apply minimal placeholder substitution needed for compile-only commands."""
    extra = re.sub(r"%then_run_(js|wasm|asmjs)\b", "", extra)

    abs_test = str(test_file.resolve())
    src_dir = str(test_file.resolve().parent)

    extra = re.sub(r"%s\b", abs_test, extra)
    extra = re.sub(r"%S\b", src_dir, extra)
    extra = re.sub(r"%t\b", str(temp_dir), extra)
    return extra


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


def compile_invocations_for_test(
    *,
    test_file: Path,
    targets: set[str],
    compiler: str,
    opt_level: str,
    cheerp_flags: str,
    asan: bool,
) -> list[CompileInvocation]:
    run_lines = iter_run_lines(test_file)
    include_dir = _tests_dir_from_this_file()

    invocations: list[CompileInvocation] = []

    for target in sorted(targets):
        token = f"compile_mode_{target}"
        segments: list[str] = []

        for rl in run_lines:
            # Determinism runs in regular mode; skip preexec-only lines.
            if "preexec_only" in rl:
                continue
            seg = _extract_compile_segment(rl, token)
            if seg is not None:
                segments.append(seg)

        for idx, seg in enumerate(segments):
            extra = seg[len(token) :].strip()
            prefix = base_compile_mode_argv(
                compiler=compiler,
                target=target,
                opt_level=opt_level,
                include_dir=include_dir,
                cheerp_flags=cheerp_flags,
                asan=asan,
            )
            invocations.append(
                CompileInvocation(
                    test_file=test_file,
                    target=target,
                    invocation_index=idx,
                    compile_prefix=prefix,
                    extra_args_template=extra,
                )
            )

    return invocations


def materialize_compile_argv(inv: CompileInvocation, temp_dir: Path) -> list[str]:
    extra = _substitute_lit_placeholders(inv.extra_args_template, inv.test_file, temp_dir)
    extra_argv = _filter_redirections(shlex.split(extra))
    return inv.compile_prefix + extra_argv


def collect_artifact_manifest_hashes(
    output_root: Path,
    target: str,
    wasm_binary_only: bool,
    wasm_determinism_mode: str,
) -> dict[str, str]:
    """Return map of relative artifact path -> hash.

    Selection is target-aware:
    - wasm + wasm_binary_only: compare wasm binaries only (e.g. *.wasm) when present
    - wasm + compare-all: compare all produced artifacts (e.g. loader + .wasm)
    - js/asmjs: compare non-wasm outputs

    Hashing mode for wasm binaries:
    - strict: hash raw bytes
    - functional: hash wasm with import/export names canonicalized away
    """
    files = [p for p in sorted(output_root.rglob("*")) if p.is_file()]

    selected: list[Path]
    if target == "wasm":
        if wasm_binary_only:
            wasm_files = [p for p in files if p.suffix == ".wasm"]
            selected = wasm_files if wasm_files else files
        else:
            selected = files
    elif target in ("js", "asmjs"):
        non_wasm_files = [p for p in files if p.suffix != ".wasm"]
        selected = non_wasm_files if non_wasm_files else files
    else:
        selected = files

    manifest: dict[str, str] = {}
    for p in selected:
        rel = str(p.relative_to(output_root))
        if target == "wasm" and p.suffix == ".wasm" and wasm_determinism_mode == "functional":
            raw = p.read_bytes()
            try:
                norm = _normalize_wasm_functional(raw)
            except Exception:
                # Fallback to strict bytes on parser issues
                norm = raw
            manifest[rel] = _md5_bytes(norm)
        else:
            manifest[rel] = _md5_file(p)
    return manifest


def summarize_manifest_diff(base: dict[str, str], other: dict[str, str], limit: int = 10) -> str:
    msgs: list[str] = []

    base_keys = set(base.keys())
    other_keys = set(other.keys())

    missing = sorted(base_keys - other_keys)
    extra = sorted(other_keys - base_keys)
    changed = sorted(k for k in (base_keys & other_keys) if base[k] != other[k])

    if missing:
        msgs.append("missing: " + ", ".join(missing[:limit]))
    if extra:
        msgs.append("extra: " + ", ".join(extra[:limit]))
    if changed:
        msgs.append("changed: " + ", ".join(changed[:limit]))

    if not msgs:
        return "no diff"
    return " | ".join(msgs)


def check_invocation_determinism(
    *,
    inv: CompileInvocation,
    level: int,
    runs: int,
    probability: float,
    print_cmd: bool,
    wasm_binary_only: bool,
    wasm_determinism_mode: str,
    tmp_root: Path,
    failures_dir: Path,
) -> tuple[bool, str]:
    test_name = _test_name_with_parent(inv.test_file)
    label = f"{test_name}:{inv.target}#{inv.invocation_index}"

    if level == 0:
        return True, f"SKIP (determinism disabled): {label}"

    if not should_test_determinism(label, probability):
        return True, f"SKIP (probability): {label}"

    if runs <= 0:
        runs = 1

    task_slug = _safe_filename(f"{test_name}_{inv.target}_{inv.invocation_index}")
    task_root = tmp_root / task_slug

    baseline_manifest: dict[str, str] | None = None
    baseline_dir: Path | None = None

    for r in range(runs):
        run_dir = task_root / f"run_{r}"
        run_dir.mkdir(parents=True, exist_ok=True)

        argv = materialize_compile_argv(inv, run_dir)
        if print_cmd:
            print("Command:", " ".join(shlex.quote(a) for a in argv))

        result = subprocess.run(
            argv,
            capture_output=True,
            text=True,
            cwd=str(inv.test_file.parent),
        )

        if result.returncode != 0:
            stderr_tail = _tail_lines(result.stderr)
            stdout_tail = _tail_lines(result.stdout)
            phase = "first" if r == 0 else f"run {r}"
            return (
                False,
                f"COMPILE FAILURE ({phase}): {label}\n"
                f"  returncode={result.returncode}\n"
                f"  stderr(tail):\n{stderr_tail}\n"
                f"  stdout(tail):\n{stdout_tail}",
            )

        manifest = collect_artifact_manifest_hashes(
            run_dir,
            inv.target,
            wasm_binary_only,
            wasm_determinism_mode,
        )
        if not manifest:
            return (
                False,
                f"NO ARTIFACTS: {label}\n"
                f"  No files were generated under {run_dir}",
            )

        if r == 0:
            baseline_manifest = manifest
            baseline_dir = run_dir
            continue

        if manifest != baseline_manifest:
            failures_dir.mkdir(parents=True, exist_ok=True)
            failure_base = failures_dir / _safe_filename(f"{test_name}.{inv.target}.{inv.invocation_index}")
            if failure_base.exists():
                shutil.rmtree(failure_base)

            run0_copy = failure_base / "run0"
            runn_copy = failure_base / f"run{r}"
            shutil.copytree(baseline_dir, run0_copy)
            shutil.copytree(run_dir, runn_copy)

            diff_summary = summarize_manifest_diff(baseline_manifest or {}, manifest)
            return (
                False,
                f"DETERMINISM FAILURE (artifacts): {label}\n"
                f"  diff: {diff_summary}\n"
                f"  saved:\n    {run0_copy}\n    {runn_copy}",
            )

    return True, f"DETERMINISM PASS (artifacts): {label}"


def find_test_files(root: Path, *, exclude_dirs: set[str]) -> list[Path]:
    tests: list[Path] = []
    for ext in ("*.cpp", "*.c"):
        tests.extend(root.rglob(ext))

    filtered: list[Path] = []
    for p in tests:
        parts = set(p.parts)
        if "Output" in parts:
            continue
        if any(part.startswith("Outputs") for part in p.parts):
            continue
        if "determinism_failures" in parts:
            continue
        if "determinism_failures_compile_only" in parts:
            continue
        if any(ex in parts for ex in exclude_dirs):
            continue
        filtered.append(p)

    return sorted(set(filtered))


def main() -> int:
    ap = argparse.ArgumentParser(description="Cheerp LIT compile-only determinism wrapper")
    ap.add_argument("test_file", nargs="?", help="Single test file to check")
    ap.add_argument("--suite", help="Directory containing test suite")
    ap.add_argument("--level", type=int, default=1, help="Determinism level: 0=off, 1=enabled")
    ap.add_argument("--runs", type=int, default=1, help="Number of compile runs to compare")
    ap.add_argument("--probability", type=float, default=1.0, help="Probability of checking each invocation")
    ap.add_argument("--jobs", type=int, default=1, help="Parallel jobs")
    ap.add_argument("--target", default="js,wasm,asmjs", help="Targets: js,wasm,asmjs (comma-separated)")
    ap.add_argument("--compiler", default="/opt/cheerp/bin/clang++", help="Compiler driver")
    ap.add_argument("--opt-level", default="-O1", help="Optimization level")
    ap.add_argument("--cheerp-flags", default="", help="Additional compiler flags")
    ap.add_argument("--asan", action="store_true", help="Enable ASAN for wasm/asmjs targets")
    ap.add_argument("--exclude-dir", action="append", default=[], help="Directory name to exclude from --suite discovery (repeatable)")
    ap.add_argument("--include-xfail", action="store_true", help="Include tests marked with // XFAIL:")
    ap.add_argument("--wasm-binary-only", dest="wasm_binary_only", action="store_true", default=True,
                    help="For wasm target, compare only .wasm artifacts (default)")
    ap.add_argument("--wasm-compare-all-artifacts", dest="wasm_binary_only", action="store_false",
                    help="For wasm target, compare all artifacts (e.g. loader + .wasm)")
    ap.add_argument("--wasm-determinism-mode", choices=("strict", "functional"), default="strict",
                    help="strict: raw wasm bytes; functional: ignore wasm import/export names")
    ap.add_argument("--print-cmd", action="store_true", help="Print commands being executed")
    ap.add_argument("--keep-temps", action="store_true", help="Keep temporary output trees")

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
        if any(ex in set(test_files[0].parts) for ex in exclude_dirs):
            print(f"Skipping excluded test file: {test_files[0]}")
            return 0
    else:
        test_files = find_test_files(Path(args.suite), exclude_dirs=exclude_dirs)

    if not test_files:
        print("No test files found")
        return 1

    available_features: set[str] = set(targets)
    available_features.add("regular")
    available_features.add("no-valgrind")
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
        if reqs and not requirements_satisfied(reqs, available_features):
            continue

        invs = compile_invocations_for_test(
            test_file=tf,
            targets=targets,
            compiler=args.compiler,
            opt_level=args.opt_level,
            cheerp_flags=args.cheerp_flags,
            asan=args.asan,
        )
        tasks.extend(invs)

    if not tasks:
        print("No compile_mode_<target> RUN lines found in selected tests")
        return 1

    jobs = max(1, int(args.jobs))
    failures_dir = _repo_root_from_this_file() / "determinism_failures_compile_only"

    # Ensure compiler-adjacent tools can be found.
    compiler_dir = str(Path(args.compiler).resolve().parent)
    os.environ["PATH"] = compiler_dir + os.pathsep + os.environ.get("PATH", "")

    tmp_root_ctx = tempfile.TemporaryDirectory(prefix="lit_det_compile_hash_")
    tmp_root = Path(tmp_root_ctx.name)

    total = len(tasks)
    passed = 0
    failed = 0
    skipped = 0

    print(f"Running {total} compile invocations with artifact-hash determinism")
    print(f"  level={args.level} runs={args.runs} probability={probability} jobs={jobs}")
    print(f"  targets={','.join(sorted(targets))}")
    if "wasm" in targets:
        artifact_mode = ".wasm only" if args.wasm_binary_only else "all wasm artifacts"
        print(f"  wasm_artifact_mode={artifact_mode}")
        print(f"  wasm_determinism_mode={args.wasm_determinism_mode}")
    if exclude_dirs:
        print(f"  exclude_dirs={','.join(sorted(exclude_dirs))}")

    def _run_one(inv: CompileInvocation) -> tuple[bool, str]:
        return check_invocation_determinism(
            inv=inv,
            level=args.level,
            runs=args.runs,
            probability=probability,
            print_cmd=args.print_cmd,
            wasm_binary_only=args.wasm_binary_only,
            wasm_determinism_mode=args.wasm_determinism_mode,
            tmp_root=tmp_root,
            failures_dir=failures_dir,
        )

    try:
        with concurrent.futures.ThreadPoolExecutor(max_workers=jobs) as ex:
            future_map = {ex.submit(_run_one, inv): inv for inv in tasks}
            for fut in concurrent.futures.as_completed(future_map):
                inv = future_map[fut]
                ok, msg = fut.result()
                label = f"{_test_name_with_parent(inv.test_file)}:{inv.target}#{inv.invocation_index}"
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
    print("Determinism summary")
    print(f"  total:   {total}")
    print(f"  passed:  {passed}")
    print(f"  failed:  {failed}")
    print(f"  skipped: {skipped}")

    return 0 if failed == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
