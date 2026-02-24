#!/usr/bin/env python3

import os
import subprocess
import sys
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
parser.add_option("--determinism-runs", dest="runs", help="Select the number of runs to compare for determinism testing", action="store", type="int", default=3)
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

# def run_determinism_tests(level = 0, probability=0.1):
#     print("Running Determinism tests")
#     print(f"level =", level)
#     print(f"probability =", probability)
#     subprocess.run(["python3", "cheerp-test/stable/helpers/determinism_wrapper.py"])

if __name__ == "__main__":
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
        if (option.determinism > 0):
            mode.append("determinism")

    if not mode:
        mode = ["wasm", "asmjs", "genericjs"]

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
        cheerp_flags.append("-cheerp-himem")
    if (option.typescript):
        cheerp_flags.append("-cheerp-generate-dts")
    
    # Print summary before running tests
    print("=== Configuration Summary ===")
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
        command = "lit -vv --param OPT_LEVEL=" + opt_level + " --param CHEERP_FLAGS='" + " ".join(cheerp_flags) + "' --param PRE_EX=j " + test_args
        if option.print_cmd:
            print(f"Command: {command}")
        result = subprocess.run(command, shell=True)
        if result.returncode != 0:
            print("Preexecute tests failed")
            exit_code = result.returncode

    if ("preexecute-asmjs" in mode):
        # run preexecute-asmjs tests
        mode.pop(mode.index("preexecute-asmjs"))
        test_args = " ".join(test_paths)
        command = "lit -vv --param OPT_LEVEL=" + opt_level + " --param CHEERP_FLAGS='" + " ".join(cheerp_flags) + "' --param PRE_EX=a " + test_args
        if option.print_cmd:
            print(f"Command: {command}")
        result = subprocess.run(command, shell=True)
        if result.returncode != 0:
            print("Preexecute-asmjs tests failed")
            exit_code = result.returncode

    if ("determinism" in mode):
        print("Running Determinism tests")
        print(f"determinism: {option.determinism}")
        print(f"determinism_runs: {option.runs}")
        print(f"determinism_probability: {option.determinism_probability}")
        mode.pop(mode.index("determinism"))
        level = option.determinism
        
        # Build command based on whether we have specific files or directories
        cmd = ["python3", "./helpers/determinism_wrapper.py", 
               f"--level={option.determinism}", 
               f"--runs={option.runs}", 
               f"--probability={option.determinism_probability}"]
        
        # Check if test_paths contains specific files or directories
        first_path = test_paths[0] if test_paths else '.'
        if os.path.isfile(first_path):
            # Run tests for each specified file
            for test_file in test_paths:
                if os.path.isfile(test_file):
                    file_result = subprocess.run(cmd + [test_file], capture_output=False, text=True)
                    if file_result.returncode != 0:
                        exit_code = file_result.returncode
        else:
            # Run tests for directory/suite
            result = subprocess.run(cmd + [f"--suite={first_path}"], capture_output=False, text=True)
            if result.returncode != 0:
                print("Determinism tests failed")
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
        lit_options.append("-vv")
        if effective_jobs > 1: 
            lit_options.append(f"-j{effective_jobs}")
        
        command = f"lit {' '.join(lit_options)} {' '.join(lit_params)} {test_args}"
        
        if option.print_cmd:
            print(f"Command: {command}\n")
        result = subprocess.run(command, shell=True)
        if result.returncode != 0:
            exit_code = result.returncode

    sys.exit(exit_code)