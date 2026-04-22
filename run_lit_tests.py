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
parser.add_option("--keep", dest="keep_logs", help="Don't delete log files for individual tests",
    action="store_true", default=False)
parser.add_option("--prefix", dest="prefix", help="Write outputs into Outputs-<prefix>/ instead of Outputs/", action="store")
parser.add_option("--asmjs", dest="asmjs", help="Run the tests in asmjs mode", action="store_true", default=False)
parser.add_option("--genericjs", dest="genericjs", help="Run the tests in genericjs mode", action="store_true", default=False)
parser.add_option("--wasm", dest="wasm", help="Run the tests in wasm mode",
    action="store_true", default=False)
parser.add_option("--typescript", dest="typescript", help="Also generate .d.ts files", action="store_true", default=False)
parser.add_option("--valgrind", dest="valgrind", help="Run with valgrind activated", action="store_true", default=False)
parser.add_option("--preexecute", dest="preexecute", help="Run the tests inside PreExecuter", action="store_true", default=False)
parser.add_option("--determinism", dest="determinism", help="After the regular run, recompile a sampled subset and diff artifacts to detect compiler non-determinism", action="store_true", default=False)
parser.add_option("--determinism-probability", dest="determinism_probability", help="Probability (0-1) that each test is included in the determinism sample", action="store", type="float", default=0.1)
parser.add_option("--determinism-only", dest="determinism_only", help="Skip the regular run and do two determinism passes (for CI)", action="store_true", default=False)
parser.add_option("--determinism-exclude-dir", dest="determinism_exclude_dirs", help="Exclude directory name from determinism sampling; repeatable", action="append", default=[])
parser.add_option("--determinism-seed", dest="determinism_seed", help="Seed string for the sampling RNG (auto-generated and printed if omitted)", action="store", default=None)
parser.add_option("--preexecute-asmjs", dest="preexecute_asmjs", help="Run the tests inside PreExecuter in asmjs mode", action="store_true", default=False)
parser.add_option("--all", dest="all", help="Run all the test kinds [genericjs/asmjs/wasm/preexecute]", action="store_true", default=False)
parser.add_option("--pretty-code", dest="pretty_code", help="Compile with -cheerp-pretty-code", action="store_true", default=False)
parser.add_option("--no-lto", dest="no_lto", help="Compile with -cheerp-no-lto", action="store_true", default=False)
parser.add_option("--print-cmd", dest="print_cmd", help="Print the commands as they're executed", action="store_true", default=False)
parser.add_option("--asan", dest="test_asan", help="Test using AddressSanitizer (only asmjm/wasm)", action="store_true", default=False)
parser.add_option("--himem", dest="himem", help="Run tests with heap start at 2GB", action="store_true", default=False)
parser.add_option("--print-stats", dest="print_stats", help="Print a summary of test result numbers", action="store_true", default=False)
parser.add_option("--time", dest="time_tests", help="Print compilation time and run time for each test", action="store_true", default=False)
parser.add_option("--compiler", dest="compiler", help="Compiler alias/path to pass to lit (e.g. cheerp, local, /opt/cheerp2/bin/clang++)", action="store")
parser.add_option("--cheerp-prefix", dest="cheerp_prefix", help="Cheerp install prefix to pass to lit via CHEERP_PREFIX", action="store")
parser.add_option("--ir", dest="emit_ir", help="Dump the LLVM IR after each pass", action="store_true", default=False);
parser.add_option("--quick", dest="quick_mode", help="run all modes in parallel (combined regular+preexec)", action="store_true", default=False);
(option, args) = parser.parse_args()
def _format_duration(seconds: float) -> str:
    # Human-friendly, stable formatting for logs
    seconds = max(0.0, float(seconds))
    ms = int(round((seconds - int(seconds)) * 1000.0))
    total = int(seconds)
    s = total % 60
    m = (total // 60) % 60
    h = total // 3600
    if h > 0:
        return f"{h:d}:{m:02d}:{s:02d}.{ms:03d}s"
    return f"{m:d}:{s:02d}.{ms:03d}s"


def _run_timed(label: str, *, command=None, argv=None, shell: bool = False, print_cmd: bool = False, **kwargs):
    """Run a subprocess while measuring wall-clock time.

    Exactly one of `command` (string) or `argv` (list) must be provided.
    Returns (CompletedProcess, elapsed_seconds).
    """
    if (command is None) == (argv is None):
        raise ValueError("Provide exactly one of command= or argv=")

    if print_cmd:
        if command is not None:
            print(f"Command: {command}")
        else:
            print(f"Command: {' '.join(argv)}")

    t0 = time.perf_counter()
    if command is not None:
        result = subprocess.run(command, shell=shell, **kwargs)
    else:
        result = subprocess.run(argv, **kwargs)
    t1 = time.perf_counter()
    return result, (t1 - t0)

def _collect_testcase_stats(xml_reports):
    stats = {
        'total': 0,
        'passed': 0,
        'failed': 0,
        'errored': 0,
        'skipped': 0,
    }
    per_test_times = []

    for xml_file in xml_reports:
        try:
            tree = ET.parse(xml_file)
            root = tree.getroot()
        except Exception:
            continue

        for testcase in root.findall('.//testcase'):
            stats['total'] += 1
            classname = testcase.get('classname', '')
            name = testcase.get('name', '')
            full_name = f"{classname}::{name}" if classname else name

            try:
                t = float(testcase.get('time', '0') or 0)
            except ValueError:
                t = 0.0
            per_test_times.append((full_name, t))

            if testcase.find('failure') is not None:
                stats['failed'] += 1
            elif testcase.find('error') is not None:
                stats['errored'] += 1
            elif testcase.find('skipped') is not None:
                stats['skipped'] += 1
            else:
                stats['passed'] += 1

    return stats, per_test_times


def _discover_and_sample(test_paths, probability, exclude_dirs, seed):
    """Expand test_paths to .cpp/.c files and sample at probability using a seeded RNG."""
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


if __name__ == "__main__":
    overall_t0 = time.perf_counter()
    timings = []  # list of dicts: {label, seconds}

    mode = []
    cheerp_flags = []
    extra_flags = []
    # Use args as test paths, default to current directory if none provided
    test_paths = args if len(args) > 0 else ['tests']
    exit_code = 0

    opt_level = f"O{option.optlevel}"

    user_lit_params = []
    if option.compiler:
        user_lit_params.append(f"--param COMPILER={shlex.quote(option.compiler)}")
    if option.cheerp_prefix:
        user_lit_params.append(f"--param CHEERP_PREFIX={shlex.quote(option.cheerp_prefix)}")
    if option.keep_logs:
        user_lit_params.append("--param KEEP_LOGS=1")
    if option.prefix:
        user_lit_params.append(f"--param OUTPUT_PREFIX={shlex.quote(option.prefix)}")
    if option.print_stats:
        user_lit_params.append("--param PRINT_STATS=1")
    if option.time_tests:
        user_lit_params.append("--param TIME_TESTS=1")
    if option.himem:
        user_lit_params.append("--param HIMEM=1")
    if option.emit_ir:
        user_lit_params.append("--param IR=1")
    if (option.all):
        mode = ["wasm", "asmjs", "genericjs", "preexecute", "preexecute-asmjs"]
    else:
        if (option.wasm):
            mode.append("wasm")
        if (option.asmjs):
            mode.append("asmjs")
        if (option.genericjs):  
            mode.append("genericjs")
        if (option.preexecute):
            mode.append("preexecute")
        if (option.preexecute_asmjs):
            mode.append("preexecute-asmjs")
        # Note: determinism is not added as a mode, it runs alongside the specified target modes

    if not mode:
        mode = ["wasm", "asmjs", "genericjs"]
    
    # Store original mode list for determinism testing (mode list gets modified during execution)
    original_mode = mode.copy()

    if (option.valgrind):
        cheerp_flags.append("-valgrind")
        extra_flags.append("--valgrind") 

    if (option.pretty_code):
        cheerp_flags.append("-cheerp-pretty-code")
    if (option.no_lto):
        cheerp_flags.append("-cheerp-no-lto")
    if (option.test_asan):
        cheerp_flags.append("-asan")
    if option.himem:
        if any(m in ("wasm", "asmjs") for m in mode):
            cheerp_flags.append("-cheerp-linear-stack-size=2048")
            cheerp_flags.append("-cheerp-linear-heap-size=2112")
        else:
            print("Warning: --himem requested but no wasm/asmjs targets are enabled; ignoring")
    if (option.typescript):
        cheerp_flags.append("-cheerp-make-dts")
    
    print("=== Configuration Overview ===")
    print(f"Test paths: {test_paths}")
    print(f"Optimization level: -O{option.optlevel}")
    print(f"Target modes: {mode}")
    print(f"determinism only mode: {option.determinism_only}")
    if option.determinism or option.determinism_only:
        print(f"Determinism enabled (probability={option.determinism_probability})")
    print(f"Jobs: {option.jobs}")

    if cheerp_flags:
        print(f"Cheerp flags: {cheerp_flags}")
    if extra_flags:
        print(f"Extra flags: {extra_flags}")
    if option.compiler:
        print(f"Compiler parameter: {option.compiler}")
    if option.cheerp_prefix:
        print(f"Cheerp prefix parameter: {option.cheerp_prefix}")
    if option.keep_logs:
        print("Keep logs: enabled")
    if option.prefix:
        print(f"Output prefix: {option.prefix}")
    if option.print_stats:
        print("Print stats: enabled")
    if option.time_tests:
        print("Per-test timing report: enabled")
    if option.emit_ir:
        print("IR dumping: enabled")

    print("="*30 + "\n")

    # Wipe the output trees we're about to populate so stale artifacts from
    # previous runs can't poison determinism comparisons (or mislead callers
    # who grep Outputs/ looking at fresh results).
    repo_root_abs = os.path.dirname(os.path.abspath(__file__))
    base_prefix = option.prefix or ""
    trees_to_clean: list[str] = []
    if option.determinism_only:
        trees_to_clean.append(f"Outputs-{base_prefix}-det-a" if base_prefix else "Outputs-det-a")
        trees_to_clean.append(f"Outputs-{base_prefix}-det-b" if base_prefix else "Outputs-det-b")
    else:
        trees_to_clean.append(f"Outputs-{base_prefix}" if base_prefix else "Outputs")
        if option.determinism:
            trees_to_clean.append(f"Outputs-{base_prefix}-det" if base_prefix else "Outputs-det")
    for tree in trees_to_clean:
        tree_path = os.path.join(repo_root_abs, tree)
        if os.path.isdir(tree_path):
            print(f"Cleaning {tree}/")
            shutil.rmtree(tree_path, ignore_errors=True)

    # Quick mode: run regular and preexec modes in a single lit invocation
    if option.quick_mode:
        if option.determinism_only:
            print("Warning: --quick mode ignored when --determinism-only is set")
        else:
            test_args = " ".join(test_paths)
            
            targets = []
            if "wasm" in mode or "genericjs" in mode:
                targets.append("wasm")
            if "asmjs" in mode:
                targets.append("asmjs")
            if "genericjs" in mode or "wasm" in mode:
                targets.append("js")
            
            if not targets:
                targets = ["wasm", "asmjs", "js"]
            
            target_param = ",".join(targets)
            
            # Build lit command with both regular and preexec modes enabled
            lit_params = []
            lit_params.append(f"--param OPT_LEVEL={opt_level}")
            lit_params.append(f"--param TARGET={target_param}")
            lit_params.append("--param REG=1")  # Enable regular mode
            lit_params.append("--param PRE_EX=1")  # Enable both preexec modes
            
            if cheerp_flags:
                flags_str = " ".join(cheerp_flags)
                lit_params.append(f"--param CHEERP_FLAGS={shlex.quote(flags_str)}")
            
            lit_params.extend(user_lit_params)
            
            lit_options = ["-vv", f"-j{option.jobs}"]
            lit_options.append("--xunit-xml-output=litTestReport.xml")
            
            command = f"lit {' '.join(lit_options)} {' '.join(lit_params)} {test_args}"
            
            result, elapsed = _run_timed(
                "lit (quick mode: regular+preexec)",
                command=command,
                shell=True,
                print_cmd=option.print_cmd,
            )
            timings.append({"label": "lit (quick mode)", "seconds": elapsed})
            print(f"[timing] lit (quick mode): {_format_duration(elapsed)}")
            if result.returncode != 0:
                exit_code = result.returncode
            
            # Clear mode list so we don't run tests again below
            mode = []

    if ("preexecute" in mode and not option.determinism_only):
        # run preexecute tests
        mode.pop(mode.index("preexecute"))
        test_args = " ".join(test_paths)
        command_parts = [
            "lit",
            "-vv",
            "--xunit-xml-output=litTestReport_preexec.xml",
            "--param OPT_LEVEL=" + opt_level,
            "--param CHEERP_FLAGS=" + shlex.quote(" ".join(cheerp_flags)),
            "--param PRE_EX=j",
            "--param REG=0",
            *user_lit_params,
            "-j" + str(option.jobs),
            test_args,
        ]
        command = " ".join(part for part in command_parts if part)
        result, elapsed = _run_timed(
            "lit (preexecute/js)",
            command=command,
            shell=True,
            print_cmd=option.print_cmd,
        )
        timings.append({"label": "lit (preexecute/js)", "seconds": elapsed})
        print(f"[timing] lit (preexecute/js): {_format_duration(elapsed)}")
        if result.returncode != 0:
            print("Preexecute tests failed")
            exit_code = result.returncode

    if ("preexecute-asmjs" in mode and not option.determinism_only):
        # run preexecute-asmjs tests
        mode.pop(mode.index("preexecute-asmjs"))
        test_args = " ".join(test_paths)
        command_parts = [
            "lit",
            "-vv",
            "--xunit-xml-output=litTestReport_preexec_asmjs.xml",
            "--param OPT_LEVEL=" + opt_level,
            "--param CHEERP_FLAGS=" + shlex.quote(" ".join(cheerp_flags)),
            "--param PRE_EX=a",
            "--param REG=0",  # Disable regular mode for preexec-only
            *user_lit_params,
            "-j" + str(option.jobs),
            test_args,
        ]
        command = " ".join(part for part in command_parts if part)
        result, elapsed = _run_timed(
            "lit (preexecute/asmjs)",
            command=command,
            shell=True,
            print_cmd=option.print_cmd,
        )
        timings.append({"label": "lit (preexecute/asmjs)", "seconds": elapsed})
        print(f"[timing] lit (preexecute/asmjs): {_format_duration(elapsed)}")
        if result.returncode != 0:
            print("Preexecute-asmjs tests failed")
            exit_code = result.returncode

    # Run tests for remaining modes
    if mode and not option.determinism_only:
        test_args = " ".join(test_paths)
        
        # Build target parameter
        targets = []
        if "wasm" in mode:
            targets.append("wasm")
        if "asmjs" in mode:
            targets.append("asmjs")
        if "genericjs" in mode:
            targets.append("js")
        
        target_param = ",".join(targets) if targets else "wasm,asmjs,js"
        
        # Build lit command
        lit_params = []
        lit_params.append(f"--param OPT_LEVEL={opt_level}")
        lit_params.append(f"--param TARGET={target_param}")
        
        if cheerp_flags:
            flags_str = " ".join(cheerp_flags)
            lit_params.append(f"--param CHEERP_FLAGS={shlex.quote(flags_str)}")

        lit_params.extend(user_lit_params)

        lit_options = []

        if option.emit_ir:
            lit_options.append("-a")     
            lit_options.append("-j1")
            # limit jobs for readable ir output
        else:
            lit_options.append("-vv")
            lit_options.append(f"-j{option.jobs}")
        
        # Add test report generation in xunit XML format
        # This creates litTestReport.xml which converts to .test format
        lit_options.append("--xunit-xml-output=litTestReport.xml")
        
        command = f"lit {' '.join(lit_options)} {' '.join(lit_params)} {test_args}"
        
        result, elapsed = _run_timed(
            f"lit (targets={target_param})",
            command=command,
            shell=True,
            print_cmd=option.print_cmd,
        )
        timings.append({"label": f"lit (targets={target_param})", "seconds": elapsed})
        print(f"[timing] lit (targets={target_param}): {_format_duration(elapsed)}")
        if result.returncode != 0:
            exit_code = result.returncode
    
    if option.determinism or option.determinism_only:
        print("\n" + "="*60)
        print("Running Determinism check")
        print(f"  Probability: {option.determinism_probability}")
        print("="*60 + "\n")

        # Targets and flags for the determinism-specific lit invocation(s).
        det_targets = []
        if "wasm" in original_mode: det_targets.append("wasm")
        if "asmjs" in original_mode: det_targets.append("asmjs")
        if "genericjs" in original_mode: det_targets.append("js")
        if not det_targets:
            det_targets = ["wasm", "asmjs", "js"]
        det_target_param = ",".join(det_targets)

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
            def _tree_path(prefix: str) -> str:
                return os.path.join(repo_root_abs, f"Outputs-{prefix}" if prefix else "Outputs")

            def _run_det_pass(prefix: str, label: str) -> int:
                """Run lit with OUTPUT_PREFIX=prefix over the sampled files."""
                lit_params = [
                    f"--param OPT_LEVEL={opt_level}",
                    f"--param TARGET={det_target_param}",
                    f"--param OUTPUT_PREFIX={shlex.quote(prefix)}",
                    "--param COMPILE_ONLY=1",
                ]
                if cheerp_flags:
                    lit_params.append(f"--param CHEERP_FLAGS={shlex.quote(' '.join(cheerp_flags))}")
                # Inherit user-supplied COMPILER/CHEERP_PREFIX/etc, but drop any OUTPUT_PREFIX
                # from user_lit_params since we're overriding it.
                for p in user_lit_params:
                    if "OUTPUT_PREFIX" not in p:
                        lit_params.append(p)
                files_part = " ".join(shlex.quote(str(p)) for p in sampled)
                cmd = f"lit -vv -j{option.jobs} {' '.join(lit_params)} {files_part}"
                result, elapsed = _run_timed(label, command=cmd, shell=True, print_cmd=option.print_cmd)
                timings.append({"label": label, "seconds": elapsed})
                print(f"[timing] {label}: {_format_duration(elapsed)}")
                return result.returncode

            if option.determinism_only:
                prefix_a = f"{base_prefix}-det-a" if base_prefix else "det-a"
                prefix_b = f"{base_prefix}-det-b" if base_prefix else "det-b"
                rc_a = _run_det_pass(prefix_a, "lit (determinism pass A)")
                rc_b = _run_det_pass(prefix_b, "lit (determinism pass B)")
                if rc_a or rc_b:
                    print("Warning: one or more determinism passes had non-zero exit; comparator will still run")
                tree_a = _tree_path(prefix_a)
                tree_b = _tree_path(prefix_b)
            else:
                det_prefix = f"{base_prefix}-det" if base_prefix else "det"
                rc = _run_det_pass(det_prefix, "lit (determinism pass)")
                if rc:
                    print("Warning: determinism pass had non-zero exit; comparator will still run")
                tree_a = _tree_path(base_prefix)
                tree_b = _tree_path(det_prefix)

            compare_cmd = [
                "python3", "./helpers/determinism_check.py",
                "--tree-a", tree_a,
                "--tree-b", tree_b,
                "--source-root", tests_source_root,
                "--failures-dir", os.path.join(repo_root_abs, "determinism_failures_compile_only"),
                *[str(p) for p in sampled],
            ]
            result, elapsed = _run_timed("determinism compare", argv=compare_cmd, shell=False, print_cmd=option.print_cmd)
            timings.append({"label": "determinism compare", "seconds": elapsed})
            print(f"[timing] determinism compare: {_format_duration(elapsed)}")
            if result.returncode != 0:
                exit_code = result.returncode
    
    # Collect all XML reports that were generated
    xml_reports = []
    for xml_file in ['litTestReport.xml', 'litTestReport_preexec.xml', 'litTestReport_preexec_asmjs.xml']:
        if os.path.exists(xml_file):
            xml_reports.append(xml_file)
    
    report_t0 = time.perf_counter()
    if xml_reports:
        try:
            import xml.etree.ElementTree as ET
            
            # Create a merged text report from all XML reports
            with open('litTestReport.test', 'w') as f:
                f.write('<testsuite>\n')
                
                for xml_file in xml_reports:
                    try:
                        tree = ET.parse(xml_file)
                        root = tree.getroot()
                        
                        for testsuite in root.findall('.//testsuite'):
                            for testcase in testsuite.findall('testcase'):
                                classname = testcase.get('classname', '')
                                name = testcase.get('name', '')
                                case_time = testcase.get('time', '0')
                                
                                # Check if test passed or failed
                                failure = testcase.find('failure')
                                if failure is not None:
                                    f.write(f'<testcase classname="{classname}" name="{name}" time="{case_time}">\n')
                                    f.write(f'  <failure type="{failure.get("type", "failure")}">')
                                    f.write(failure.text if failure.text else '')
                                    f.write('</failure>\n')
                                    f.write('</testcase>\n')
                                else:
                                    f.write(f'<testcase classname="{classname}" name="{name}" time="{case_time}"/>\n')
                    except Exception as e:
                        print(f"Warning: Failed to parse {xml_file}: {e}")
                
                f.write('</testsuite>\n')
            
            print(f"Test report generated: litTestReport.test (merged {len(xml_reports)} reports)")
        except Exception as e:
            print(f"Warning: Failed to convert test reports: {e}")
            # Create an empty report so CI doesn't fail
            with open('litTestReport.test', 'w') as f:
                f.write('<testsuite>\n</testsuite>\n')
    else:
        print("Warning: No test reports generated, creating empty report")
        with open('litTestReport.test', 'w') as f:
            f.write('<testsuite>\n</testsuite>\n')

    testcase_stats, per_test_times = _collect_testcase_stats(xml_reports)

    if not option.keep_logs:
        for xml_file in xml_reports:
            try:
                os.remove(xml_file)
            except OSError:
                pass

    report_elapsed = time.perf_counter() - report_t0
    timings.append({"label": "report conversion", "seconds": report_elapsed})

    if option.print_stats:
        print("\nTest stats")
        print("-" * 30)
        print(f"passed: {testcase_stats['passed']}")
        print(f"failed: {testcase_stats['failed']}")
        print(f"errored: {testcase_stats['errored']}")
        print(f"skipped/unsupported: {testcase_stats['skipped']}")
        print(f"total: {testcase_stats['total']}")

    if option.time_tests and per_test_times:
        print("\nPer-test durations")
        print("-" * 30)
        for test_name, sec in sorted(per_test_times, key=lambda x: x[1], reverse=True):
            print(f"{sec:8.3f}s  {test_name}")

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
