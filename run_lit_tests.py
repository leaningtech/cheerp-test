#!/usr/bin/env python3

import os
import random
import secrets
import shlex
import shutil
import subprocess
import sys
import time
import xml.etree.ElementTree as ET
from optparse import OptionParser
from pathlib import Path

usage = "usage: %prog [options] [test_paths...]"
parser = OptionParser(usage=usage)
parser.add_option("-O", dest="optlevel", help="Optimization level (default -O1)", action="store", type="int", default=1)
parser.add_option("-j", dest="jobs", help="Number of jobs (default 1)", action="store", type="int", default=1)
parser.add_option("--prefix", dest="prefix", help="Write outputs into Outputs-<prefix>/ instead of Outputs/", action="store")
parser.add_option("--asmjs", dest="asmjs", help="Run the asmjs target", action="store_true", default=False)
parser.add_option("--genericjs", dest="genericjs", help="Run the generic js target", action="store_true", default=False)
parser.add_option("--wasm", dest="wasm", help="Run the wasm target", action="store_true", default=False)
parser.add_option("--typescript", dest="typescript", help="Also generate .d.ts files", action="store_true", default=False)
parser.add_option("--valgrind", dest="valgrind", help="Run with valgrind activated", action="store_true", default=False)
parser.add_option("--preexecute", dest="preexecute", help="Also run preexec mode for the selected targets (skips wasm)", action="store_true", default=False)
parser.add_option("--determinism", dest="determinism", help="After each (target, mode) pass, recompile a sampled subset and diff artifacts to detect compiler non-determinism", action="store_true", default=False)
parser.add_option("--determinism-probability", dest="determinism_probability", help="Probability (0-1) that each test is included in the determinism sample", action="store", type="float", default=0.1)
parser.add_option("--determinism-only", dest="determinism_only", help="Skip the regular run and do two determinism passes per (target, mode) (for CI)", action="store_true", default=False)
parser.add_option("--determinism-exclude-dir", dest="determinism_exclude_dirs", help="Exclude directory name from determinism sampling; repeatable", action="append", default=[])
parser.add_option("--determinism-seed", dest="determinism_seed", help="Seed string for the sampling RNG (auto-generated and printed if omitted)", action="store", default=None)
parser.add_option("--all", dest="all", help="Run all targets in regular mode plus preexec mode (skipping wasm preexec)", action="store_true", default=False)
parser.add_option("--pretty-code", dest="pretty_code", help="Compile with -cheerp-pretty-code", action="store_true", default=False)
parser.add_option("--no-lto", dest="no_lto", help="Compile with -cheerp-no-lto", action="store_true", default=False)
parser.add_option("--asan", dest="test_asan", help="Test using AddressSanitizer (only asmjs/wasm)", action="store_true", default=False)
parser.add_option("--himem", dest="himem", help="Run tests with heap start at 2GB", action="store_true", default=False)
parser.add_option("--compiler", dest="compiler", help="Path to clang++ (default /opt/cheerp/bin/clang++)", action="store")
parser.add_option("--ir", dest="emit_ir", help="Dump the LLVM IR after each pass", action="store_true", default=False)
(option, args) = parser.parse_args()


def _format_duration(seconds: float) -> str:
    seconds = max(0.0, float(seconds))
    ms = int(round((seconds - int(seconds)) * 1000.0))
    total = int(seconds)
    s = total % 60
    m = (total // 60) % 60
    h = total // 3600
    if h > 0:
        return f"{h:d}:{m:02d}:{s:02d}.{ms:03d}s"
    return f"{m:d}:{s:02d}.{ms:03d}s"


def _run_timed(label, *, command=None, argv=None, shell=False, **kwargs):
    if (command is None) == (argv is None):
        raise ValueError("Provide exactly one of command= or argv=")
    t0 = time.perf_counter()
    if command is not None:
        result = subprocess.run(command, shell=shell, **kwargs)
    else:
        result = subprocess.run(argv, **kwargs)
    return result, (time.perf_counter() - t0)


def _xml_stats(xml_file):
    stats = {'total': 0, 'passed': 0, 'failed': 0, 'errored': 0, 'skipped': 0}
    try:
        root = ET.parse(xml_file).getroot()
    except Exception:
        return stats
    for tc in root.findall('.//testcase'):
        stats['total'] += 1
        if tc.find('failure') is not None:
            stats['failed'] += 1
        elif tc.find('error') is not None:
            stats['errored'] += 1
        elif tc.find('skipped') is not None:
            stats['skipped'] += 1
        else:
            stats['passed'] += 1
    return stats


def _discover_and_sample(test_paths, probability, exclude_dirs, seed):
    exclude = set(exclude_dirs)
    candidates = []
    for root in test_paths:
        rp = Path(root)
        if rp.is_file():
            candidates.append(rp)
        elif rp.is_dir():
            for ext in ("*.cpp", "*.c"):
                for p in rp.rglob(ext):
                    if any(part in exclude for part in p.parts):
                        continue
                    candidates.append(p)
    candidates = sorted(set(candidates))
    probability = max(0.0, min(1.0, probability))
    rng = random.Random(seed)
    sampled = [p for p in candidates if rng.random() < probability]
    return candidates, sampled


def _select_combos(option):
    """Resolve CLI flags into the list of (target, mode) tuples to run."""
    if option.all:
        targets = ['js', 'wasm', 'asmjs']
        modes = ['regular', 'preexec']
    else:
        targets = []
        if option.genericjs: targets.append('js')
        if option.wasm: targets.append('wasm')
        if option.asmjs: targets.append('asmjs')
        if not targets:
            targets = ['js', 'wasm', 'asmjs']
        modes = ['regular']
        if option.preexecute:
            modes.append('preexec')
    combos = [(t, m) for t in targets for m in modes if not (t == 'wasm' and m == 'preexec')]
    return combos


if __name__ == "__main__":
    overall_t0 = time.perf_counter()
    timings = []  # list of dicts: {label, seconds}

    cheerp_flags = []
    test_paths = args if args else ['tests']
    exit_code = 0

    opt_level = f"O{option.optlevel}"

    user_lit_params = []
    if option.compiler:
        user_lit_params.append(f"--param COMPILER={shlex.quote(option.compiler)}")

    if option.valgrind:
        cheerp_flags.append("-valgrind")
    if option.pretty_code:
        cheerp_flags.append("-cheerp-pretty-code")
    if option.no_lto:
        cheerp_flags.append("-cheerp-no-lto")
    if option.test_asan:
        cheerp_flags.append("-asan")
    if option.himem:
        cheerp_flags.append("-cheerp-linear-stack-size=2048")
        cheerp_flags.append("-cheerp-linear-heap-size=2112")
    if option.typescript:
        cheerp_flags.append("-cheerp-make-dts")
    if option.emit_ir:
        cheerp_flags.append("-mllvm")
        cheerp_flags.append("-print-after-all")

    combos = _select_combos(option)
    if option.test_asan:
        combos = [(t, m) for t, m in combos if t != 'js']
        if not combos:
            print("Error: --asan removes JS, but no other target was selected", file=sys.stderr)
            sys.exit(1)

    print("=== Configuration Overview ===")
    print(f"Test paths: {test_paths}")
    print(f"Optimization level: -O{option.optlevel}")
    print(f"(target, mode) combos: {combos}")
    if option.determinism or option.determinism_only:
        print(f"Determinism enabled (probability={option.determinism_probability})")
    print(f"Jobs: {option.jobs}")
    if cheerp_flags:
        print(f"Cheerp flags: {cheerp_flags}")
    if option.compiler:
        print(f"Compiler: {option.compiler}")
    if option.prefix:
        print(f"Output prefix: {option.prefix}")
    if option.emit_ir:
        print("IR dumping: enabled")
    print("=" * 30 + "\n")

    repo_root_abs = os.path.dirname(os.path.abspath(__file__))
    base_prefix = option.prefix or ""

    def _outputs_root(prefix):
        return os.path.join(repo_root_abs, f"Outputs-{prefix}" if prefix else "Outputs")

    # Wipe trees we're about to populate so stale artifacts can't poison
    # determinism comparisons or mislead callers grepping Outputs/.
    trees_to_clean = []
    if option.determinism_only:
        for combo in combos:
            t, m = combo
            trees_to_clean.append(_outputs_root(f"{base_prefix}-det-a" if base_prefix else "det-a"))
            trees_to_clean.append(_outputs_root(f"{base_prefix}-det-b" if base_prefix else "det-b"))
    else:
        trees_to_clean.append(_outputs_root(base_prefix))
        if option.determinism:
            trees_to_clean.append(_outputs_root(f"{base_prefix}-det" if base_prefix else "det"))
    for tree in dict.fromkeys(trees_to_clean):  # de-dup, preserve order
        if os.path.isdir(tree):
            print(f"Cleaning {os.path.relpath(tree, repo_root_abs)}/")
            shutil.rmtree(tree, ignore_errors=True)

    def _build_lit_params(target, mode, output_prefix, *, compile_only=False):
        params = [
            f"--param OPT_LEVEL={opt_level}",
            f"--param TARGET={target}",
            f"--param MODE={mode}",
        ]
        if output_prefix:
            params.append(f"--param OUTPUT_PREFIX={shlex.quote(output_prefix)}")
        if compile_only:
            params.append("--param COMPILE_ONLY=1")
        if cheerp_flags:
            params.append(f"--param CHEERP_FLAGS={shlex.quote(' '.join(cheerp_flags))}")
        # Inherit user-supplied params (skip OUTPUT_PREFIX since we set it).
        for p in user_lit_params:
            if "OUTPUT_PREFIX" not in p:
                params.append(p)
        return params

    def _run_lit(target, mode, output_prefix, test_args, xunit_xml, label, *, compile_only=False):
        params = _build_lit_params(target, mode, output_prefix, compile_only=compile_only)
        opts = []
        if option.emit_ir:
            opts.append("-a")
            opts.append("-j1")
        else:
            opts.append("-vv")
            opts.append(f"-j{option.jobs}")
        if xunit_xml:
            opts.append(f"--xunit-xml-output={xunit_xml}")
        cmd = f"lit {' '.join(opts)} {' '.join(params)} {test_args}"
        result, elapsed = _run_timed(label, command=cmd, shell=True)
        timings.append({"label": label, "seconds": elapsed})
        print(f"[timing] {label}: {_format_duration(elapsed)}")
        return result.returncode

    test_args = " ".join(shlex.quote(p) for p in test_paths)
    combo_reports = []  # list of (target, mode, xunit_xml_path)

    if not option.determinism_only:
        for target, mode in combos:
            combo_dir = os.path.join(_outputs_root(base_prefix), f"{target}-{mode}")
            os.makedirs(combo_dir, exist_ok=True)
            xunit = os.path.join(combo_dir, "litTestReport.xml")
            label = f"lit ({target}, {mode})"
            rc = _run_lit(target, mode, base_prefix, test_args, xunit, label)
            if rc != 0:
                exit_code = rc
            combo_reports.append((target, mode, xunit))

    if option.determinism or option.determinism_only:
        print("\n" + "=" * 60)
        print("Running Determinism check")
        print(f"  Probability: {option.determinism_probability}")
        print("=" * 60 + "\n")

        if option.determinism_seed is not None:
            seed = option.determinism_seed
            print(f"Determinism seed: {seed!r}")
        else:
            seed = secrets.token_hex(8)
            print(f"Determinism seed: {seed!r} (auto-generated; pass --determinism-seed={seed} to reproduce)")

        candidates, sampled = _discover_and_sample(
            test_paths,
            option.determinism_probability,
            option.determinism_exclude_dirs,
            seed,
        )
        print(f"Determinism: sampled {len(sampled)}/{len(candidates)} tests")

        if not sampled:
            print("Determinism: empty sample, nothing to compare")
        else:
            tests_source_root = os.path.join(repo_root_abs, "tests")
            files_part = " ".join(shlex.quote(str(p)) for p in sampled)

            for target, mode in combos:
                if option.determinism_only:
                    prefix_a = f"{base_prefix}-det-a" if base_prefix else "det-a"
                    prefix_b = f"{base_prefix}-det-b" if base_prefix else "det-b"
                    rc_a = _run_lit(target, mode, prefix_a, files_part, None,
                                    f"determinism A ({target}, {mode})", compile_only=True)
                    rc_b = _run_lit(target, mode, prefix_b, files_part, None,
                                    f"determinism B ({target}, {mode})", compile_only=True)
                    if rc_a or rc_b:
                        print(f"Warning: determinism passes had non-zero exit for ({target}, {mode}); comparator will still run")
                    tree_a = os.path.join(_outputs_root(prefix_a), f"{target}-{mode}")
                    tree_b = os.path.join(_outputs_root(prefix_b), f"{target}-{mode}")
                else:
                    det_prefix = f"{base_prefix}-det" if base_prefix else "det"
                    rc = _run_lit(target, mode, det_prefix, files_part, None,
                                  f"determinism ({target}, {mode})", compile_only=True)
                    if rc:
                        print(f"Warning: determinism pass had non-zero exit for ({target}, {mode}); comparator will still run")
                    tree_a = os.path.join(_outputs_root(base_prefix), f"{target}-{mode}")
                    tree_b = os.path.join(_outputs_root(det_prefix), f"{target}-{mode}")

                compare_cmd = [
                    "python3", "./helpers/determinism_check.py",
                    "--tree-a", tree_a,
                    "--tree-b", tree_b,
                    "--source-root", tests_source_root,
                    *[str(p) for p in sampled],
                ]
                result, elapsed = _run_timed(
                    f"determinism compare ({target}, {mode})",
                    argv=compare_cmd, shell=False,
                )
                timings.append({"label": f"determinism compare ({target}, {mode})", "seconds": elapsed})
                print(f"[timing] determinism compare ({target}, {mode}): {_format_duration(elapsed)}")
                if result.returncode != 0:
                    exit_code = result.returncode

    # Cumulative summary across (target, mode) combos.
    combo_reports = [(t, m, x) for (t, m, x) in combo_reports if os.path.exists(x)]
    if len(combo_reports) > 1:
        per_combo = [(t, m, _xml_stats(x)) for (t, m, x) in combo_reports]
        label_w = max(len(f"{t} {m}") for (t, m, _) in per_combo)
        cols = ('passed', 'failed', 'errored', 'skipped', 'total')
        widths = {c: max(len(c), max(len(str(s[c])) for (_, _, s) in per_combo)) for c in cols}
        print("\n" + "=" * 60)
        print("Cumulative summary")
        print("=" * 60)
        header = " " * label_w + "  " + "  ".join(c.rjust(widths[c]) for c in cols)
        print(header)
        for t, m, s in per_combo:
            row = f"{t} {m}".ljust(label_w) + "  " + "  ".join(str(s[c]).rjust(widths[c]) for c in cols)
            print(row)
        totals = {c: sum(s[c] for (_, _, s) in per_combo) for c in cols}
        widths = {c: max(widths[c], len(str(totals[c]))) for c in cols}
        print("-" * len(header))
        print("TOTAL".ljust(label_w) + "  " + "  ".join(str(totals[c]).rjust(widths[c]) for c in cols))

    overall_elapsed = time.perf_counter() - overall_t0
    test_elapsed = sum(t["seconds"] for t in timings if t["label"].startswith("lit") or t["label"].startswith("determinism"))

    print("\n" + "*" * 60)
    print("Timing summary")
    print("*" * 60)
    print(f"Total elapsed:       {_format_duration(overall_elapsed)}")
    print(f"Tests elapsed:       {_format_duration(test_elapsed)}")
    print(f"Non-test overhead:   {_format_duration(overall_elapsed - test_elapsed)}")
    print("\nBreakdown:")
    for t in timings:
        print(f"  - {t['label']}: {_format_duration(t['seconds'])}")

    sys.exit(exit_code)
