#!/usr/bin/env python3

import os
import subprocess
import sys
import time
from optparse import OptionParser

usage = "usage: %prog [options] [test_paths...]"
parser = OptionParser(usage=usage)
parser.add_option("-O", dest="optlevel", help="Optimization level (default -O1)", action="store", type="int", default=1)
parser.add_option("-j", dest="jobs", help="Number of jobs (default 1)", action="store", type="int", default=1)
parser.add_option("--keep", dest="keep_logs", help="Don't delete log files for individual tests",
    action="store_true", default=False)
parser.add_option("--prefix", dest="prefix", help="Keep the generated output for each test, with the name prefix_testname.js", action="store")
parser.add_option("--asmjs", dest="asmjs", help="Run the tests in asmjs mode", action="store_true", default=False)
parser.add_option("--genericjs", dest="genericjs", help="Run the tests in genericjs mode", action="store_true", default=False)
parser.add_option("--wasm", dest="wasm", help="Run the tests in wasm mode",
    action="store_true", default=False)
parser.add_option("--typescript", dest="typescript", help="Also generate .d.ts files", action="store_true", default=False)
parser.add_option("--valgrind", dest="valgrind", help="Run with valgrind activated", action="store_true", default=False)
parser.add_option("--preexecute", dest="preexecute", help="Run the tests inside PreExecuter", action="store_true", default=False)
parser.add_option("--determinism", dest="determinism", help="Select the level of testing devoted to uncover non-deterministic behaviour", action="store", type="int", default=0)
parser.add_option("--determinism-probability", dest="determinism_probability", help="Select the chance a given test is tested for determinism", action="store", type="float", default=0.1)
# parser.add_option("--determinism-wrapper", dest="determinism_wrapper", help="Select determinism checker: outputs (default), print-after, or both", action="store", type="string", default="outputs")
parser.add_option("--determinism-exclude-dir",dest="determinism_exclude_dirs",help="Exclude directory (by name) from print-after determinism suite discovery; repeatable (e.g. --determinism-exclude-dir=threading)", action="append", default=[])
parser.add_option("--preexecute-asmjs", dest="preexecute_asmjs", help="Run the tests inside PreExecuter in asmjs mode", action="store_true", default=False)
parser.add_option("--all", dest="all", help="Run all the test kinds [genericjs/asmjs/wasm/preexecute]", action="store_true", default=False)
parser.add_option("--pretty-code", dest="pretty_code", help="Compile with -cheerp-pretty-code", action="store_true", default=False)
parser.add_option("--no-lto", dest="no_lto", help="Compile with -cheerp-no-lto", action="store_true", default=False)
parser.add_option("--print-cmd", dest="print_cmd", help="Print the commands as they're executed", action="store_true", default=False)
parser.add_option("--asan", dest="test_asan", help="Test using AddressSanitizer (only asmjm/wasm)", action="store_true", default=False)
parser.add_option("--himem", dest="himem", help="Run tests with heap start at 2GB", action="store_true", default=False)
parser.add_option("--print-stats", dest="print_stats", help="Print a summary of test result numbers", action="store_true", default=False)
parser.add_option("--time", dest="time_tests", help="Print compilation time and run time for each test", action="store_true", default=False)
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

# def run_determinism_tests(level = 0, probability=0.1):
#     print("Running Determinism tests")
#     print(f"level =", level)
#     print(f"probability =", probability)
#     subprocess.run(["python3", "cheerp-test/stable/helpers/determinism_wrapper.py"])

if __name__ == "__main__":
    overall_t0 = time.perf_counter()
    timings = []  # list of dicts: {label, seconds}

    mode = []
    cheerp_flags = []
    extra_flags = []
    # Use args as test paths, default to current directory if none provided
    test_paths = args if len(args) > 0 else ['.']
    exit_code = 0

    opt_level = f"O{option.optlevel}"

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
    if (option.himem):
        if (mode in ["wasm", "asmjs"]):
            cheerp_flags.append("-cheerp-linear-stack-size=2048")
            cheerp_flags.append("-cheerp-linear-heap-size=2112")
    if (option.typescript):
        cheerp_flags.append("-cheerp-generate-dts")
    
    # Print summary before running tests
    print("=== Configuration Overview ===")
    print(f"Test paths: {test_paths}")
    print(f"Optimization level: -O{option.optlevel}")
    print(f"Target modes: {mode}")
    effective_jobs = option.jobs
    if option.test_asan:
        effective_jobs = min(option.jobs, 2)
        if effective_jobs != option.jobs:
            print(f"ASan testing detected, limiting jobs to {effective_jobs} to reduce memory usage")

    print(f"Jobs: {option.jobs}")

    if cheerp_flags:
        print(f"Cheerp flags: {cheerp_flags}")
    if extra_flags:
        print(f"Extra flags: {extra_flags}")
    print("="*30 + "\n")

    if ("preexecute" in mode):
        # run preexecute tests
        mode.pop(mode.index("preexecute"))
        test_args = " ".join(test_paths)
        command = "lit --xunit-xml-output=litTestReport_preexec.xml --param OPT_LEVEL=" + opt_level + " --param CHEERP_FLAGS='" + " ".join(cheerp_flags) + "' --param PRE_EX=j " + " -j".join(effective_jobs) + " " + test_args
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

    if ("preexecute-asmjs" in mode):
        # run preexecute-asmjs tests
        mode.pop(mode.index("preexecute-asmjs"))
        test_args = " ".join(test_paths)
        command = "lit --xunit-xml-output=litTestReport_preexec_asmjs.xml --param OPT_LEVEL=" + opt_level + " --param CHEERP_FLAGS='" + " ".join(cheerp_flags) + "' --param PRE_EX=a " + " -j".join(effective_jobs) + " " + test_args
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
    if mode:
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
            lit_params.append(f"--param CHEERP_FLAGS='{flags_str}'")

        lit_options = []
        # lit_options.append("-vv")
        lit_options.append(f"-j{effective_jobs}")
        
        # Add test report generation in xunit XML format
        # This creates litTestReport.xml which we'll convert to .test format
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
    
    # Run determinism tests if requested
    # This runs after regular tests to match old suite behavior where determinism
    # testing is integrated with the specified target mode
    if option.determinism:
        print("\n" + "="*60)
        print("Running Determinism tests")
        print(f"  Runs per test: {option.determinism}")
        print(f"  Probability: {option.determinism_probability}")
        print("="*60 + "\n")

        # Map modes to determinism wrapper target format (use original_mode since mode list was modified)
        determinism_targets = []
        if "wasm" in original_mode:
            determinism_targets.append("wasm")
        if "asmjs" in original_mode:
            determinism_targets.append("asmjs")
        if "genericjs" in original_mode:
            determinism_targets.append("js")

        print(f"Determinism testing targets: {len(determinism_targets) if determinism_targets else 'all'}")

        # Compiler-only flags for determinism wrappers (strip pseudo-flags consumed by lit.cfg)
        cheerp_flags_compiler_only = [f for f in cheerp_flags if f not in ("-valgrind", "-asan")]
        cheerp_flags_compiler_str = " ".join(cheerp_flags_compiler_only)

        # Helper to run a determinism wrapper against either a suite dir or explicit files
        def run_wrapper(wrapper_label: str, base_cmd: list[str], current_exit_code: int) -> int:
            first_path = test_paths[0] if test_paths else "."
            if os.path.isfile(first_path):
                for test_file in test_paths:
                    if not os.path.isfile(test_file):
                        continue
                    file_result, elapsed = _run_timed(
                        f"{wrapper_label} ({os.path.basename(test_file)})",
                        argv=base_cmd + [test_file],
                        shell=False,
                        print_cmd=option.print_cmd,
                        capture_output=False,
                        text=True,
                    )
                    timings.append({"label": f"{wrapper_label} ({os.path.basename(test_file)})", "seconds": elapsed})
                    print(f"[timing] {wrapper_label} ({os.path.basename(test_file)}): {_format_duration(elapsed)}")
                    if file_result.returncode != 0:
                        current_exit_code = file_result.returncode
            else:
                result, elapsed = _run_timed(
                    f"{wrapper_label} (suite={first_path})",
                    argv=base_cmd + [f"--suite={first_path}"],
                    shell=False,
                    print_cmd=option.print_cmd,
                    capture_output=False,
                    text=True,
                )
                timings.append({"label": f"{wrapper_label} (suite={first_path})", "seconds": elapsed})
                print(f"[timing] {wrapper_label} (suite={first_path}): {_format_duration(elapsed)}")
                if result.returncode != 0:
                    print(f"{wrapper_label} tests failed")
                    current_exit_code = result.returncode
            return current_exit_code

        cmd_outputs = [
            "python3",
            "./helpers/determinism_wrapper.py",
            f"--runs={option.determinism}",
            # f"--runs={option.runs}",
            f"--probability={option.determinism_probability}",
        ]
        if determinism_targets:
            cmd_outputs.append(f"--target={','.join(determinism_targets)}")
        exit_code = run_wrapper("determinism(outputs)", cmd_outputs, exit_code)

        # New print-after based wrapper
    #     if wrapper_mode in ("print-after", "both"):
    #         cmd_print_after = [
    #             "python3",
    #             "./helpers/determinism_wrapper_print_after.py",
    #             f"--level={option.determinism}",
    #             f"--runs={option.runs}",
    #             f"--probability={option.determinism_probability}",
    #             f"--jobs={effective_jobs}",
    #             f"--opt-level=-O{option.optlevel}",
    #         ]
    #         if cheerp_flags_compiler_str:
    #             cmd_print_after += ["--cheerp-flags", cheerp_flags_compiler_str]
    #         if option.test_asan:
    #             cmd_print_after.append("--asan")
    #         if determinism_targets:
    #             cmd_print_after.append(f"--target={','.join(determinism_targets)}")
    #         for d in (option.determinism_exclude_dirs or []):
    #             if d:
    #                 cmd_print_after.append(f"--exclude-dir={d}")
    #         if option.print_cmd:
    #             cmd_print_after.append("--print-cmd")
    #         exit_code = run_wrapper("determinism(print-after)", cmd_print_after, exit_code)
    
    # # The .test format is what the old test suite used
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
                
                # Process each XML report
                for xml_file in xml_reports:
                    try:
                        tree = ET.parse(xml_file)
                        root = tree.getroot()
                        
                        # Iterate through test cases
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
        # No XML reports found, create empty report
        print("Warning: No test reports generated, creating empty report")
        with open('litTestReport.test', 'w') as f:
            f.write('<testsuite>\n</testsuite>\n')

    report_elapsed = time.perf_counter() - report_t0
    timings.append({"label": "report conversion", "seconds": report_elapsed})

    overall_elapsed = time.perf_counter() - overall_t0
    test_elapsed = sum(t["seconds"] for t in timings if t["label"].startswith("lit") or t["label"].startswith("determinism"))

    print("\n" + "=" * 60)
    print("Timing summary")
    print("=" * 60)
    print(f"Total elapsed:       {_format_duration(overall_elapsed)}")
    print(f"Tests elapsed:       {_format_duration(test_elapsed)}")
    print(f"Non-test overhead:   {_format_duration(overall_elapsed - test_elapsed)}")
    print("\nBreakdown:")
    for t in timings:
        print(f"  - {t['label']}: {_format_duration(t['seconds'])}")

    sys.exit(exit_code)