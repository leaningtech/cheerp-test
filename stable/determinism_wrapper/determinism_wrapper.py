#!/usr/bin/env python3
"""
Determinism Testing Wrapper for Cheerp LIT Tests

This script runs LIT tests multiple times and checks that the compiler
produces identical output each time, detecting non-deterministic behavior.

Usage:
    python3 determinism_wrapper.py <test_file>           # Single test
    python3 determinism_wrapper.py --suite <directory>   # Entire suite
    
Environment Variables:
    DETERMINISM_LEVEL: 0=off, 1=basic output (default), 2+=pass dumps
    DETERMINISM_RUNS: Number of compilation runs to compare (default=3)
    DETERMINISM_PROBABILITY: Probability of checking a test (0.0-1.0, default=1.0)
    LIT_PARAMS: Additional LIT parameters to pass through
"""

import hashlib
import subprocess
import sys
import os
import tempfile
import shutil
import argparse
import random
import glob
from pathlib import Path

def compute_hash(filepath):
    """Compute MD5 hash of a file"""
    if not os.path.exists(filepath):
        return None
    with open(filepath, 'rb') as f:
        return hashlib.md5(f.read()).hexdigest()

def compute_hash_from_content(content):
    """Compute MD5 hash from bytes content"""
    return hashlib.md5(content).hexdigest()

def should_test_determinism(probability):
    """Decide whether to test this run based on probability"""
    return random.random() < probability

def run_test_with_determinism(test_file, lit_config_params, determinism_level=1, num_runs=3, verbose=False):
    """
    Run a test multiple times and check output determinism
    
    Args:
        test_file: Path to the test file
        lit_config_params: Dictionary of LIT configuration parameters
        determinism_level: 0=off, 1=basic output, 2+=pass dumps
        num_runs: Number of times to compile and compare
        verbose: Print detailed output
    
    Returns:
        (success: bool, message: str)
    """
    if determinism_level == 0:
        if verbose:
            print(f"SKIP (determinism disabled): {test_file}")
        return (True, "Determinism testing disabled")
    
    hashes = {}
    outputs = {}
    
    for run in range(num_runs):
        try:
            # Build LIT command
            env = os.environ.copy()
            
            # Build lit parameters
            lit_params = []
            
            # Add DETERMINISM and DETERMINISM_RUN parameters
            lit_params.append(f'--param=DETERMINISM={determinism_level}')
            lit_params.append(f'--param=DETERMINISM_RUN={run}')
            
            # Add user-provided parameters
            for key, value in lit_config_params.items():
                lit_params.append(f'--param={key}={value}')
            
            # Run LIT test - output goes to Output/<test>.tmp/run_<N>/
            # Use absolute path to ensure LIT finds the test
            abs_test_file = os.path.abspath(test_file)
            lit_cmd = ['lit', '-v'] + lit_params + [abs_test_file]
            
            if verbose:
                print(f"  Run {run}: {' '.join(lit_cmd)}")
            
            result = subprocess.run(
                lit_cmd,
                env=env,
                capture_output=True,
                text=True,
                cwd=os.path.dirname(os.path.abspath(test_file))
            )
            
            if result.returncode != 0:
                # Test failed to compile/run
                if run == 0:
                    # First run failed - this is a legitimate test failure, not determinism issue
                    return (False, f"Test failed on first run: {result.stderr}")
                else:
                    # Subsequent run failed when first succeeded - this IS a determinism issue
                    return (False, f"Test failed on run {run} after succeeding on run 0 (non-deterministic failure)")
            
            # Look for output files in the run-specific subdirectory
            test_dir = os.path.dirname(os.path.abspath(test_file))
            test_name = os.path.basename(test_file)
            
            # Construct path to run-specific output directory
            # Pattern: <test_dir>/Output/<test_name>.tmp/run_<N>/
            run_output_dir = os.path.join(test_dir, 'Output', f'{test_name}.tmp', f'run_{run}')
            
            if not os.path.exists(run_output_dir):
                if verbose:
                    print(f"  Warning: Output directory not found: {run_output_dir}")
                continue
            
            # Hash all output files in this run's directory
            found_outputs = False
            for root, dirs, files in os.walk(run_output_dir):
                for f in files:
                    if f.endswith(('.js', '.wasm', '.mjs', '.wat')):
                        filepath = os.path.join(root, f)
                        filehash = compute_hash(filepath)
                        
                        # Use filename as key (since all runs have same structure)
                        key = f
                        found_outputs = True
                        
                        if key not in hashes:
                            hashes[key] = filehash
                            # Save first output for comparison
                            with open(filepath, 'rb') as src:
                                outputs[key] = src.read()
                        else:
                            if hashes[key] != filehash:
                                    # Determinism failure detected!
                                    error_msg = f"DETERMINISM FAILURE: {test_file}\n"
                                    error_msg += f"  File: {key}\n"
                                    error_msg += f"  Hash run 0: {hashes[key]}\n"
                                    error_msg += f"  Hash run {run}: {filehash}\n"
                                    
                                    # Save differing outputs for inspection
                                    base_name = os.path.basename(test_file)
                                    diff_dir = os.path.join(test_dir, 'determinism_failures')
                                    os.makedirs(diff_dir, exist_ok=True)
                                    
                                    out0_path = os.path.join(diff_dir, f'{base_name}.run0.{os.path.basename(key)}')
                                    out1_path = os.path.join(diff_dir, f'{base_name}.run{run}.{os.path.basename(key)}')
                                    
                                    with open(out0_path, 'wb') as out:
                                        out.write(outputs[key])
                                    with open(out1_path, 'wb') as out:
                                        with open(filepath, 'rb') as src:
                                            out.write(src.read())
                                    
                                    error_msg += f"  Saved outputs:\n    {out0_path}\n    {out1_path}\n"
                                    
                                    return (False, error_msg)
            
            if not found_outputs:
                if verbose:
                    print(f"  Warning: No output files found for {test_file}")
        
        except Exception as e:
            return (False, f"Error during run {run}: {str(e)}")
    
    return (True, f"DETERMINISM PASS: {test_file}")

def find_test_files(directory):
    """Find all test files in a directory"""
    test_files = []
    for ext in ['*.cpp', '*.c']:
        test_files.extend(glob.glob(os.path.join(directory, ext)))
    return sorted(test_files)

def main():
    parser = argparse.ArgumentParser(
        description='Run Cheerp tests with determinism checking',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__
    )
    parser.add_argument('test_file', nargs='?', help='Single test file to check')
    parser.add_argument('--suite', help='Directory containing test suite')
    parser.add_argument('--level', type=int, 
                       default=int(os.environ.get('DETERMINISM_LEVEL', '1')),
                       help='Determinism level: 0=off, 1=basic (default), 2+=pass dumps')
    parser.add_argument('--runs', type=int,
                       default=int(os.environ.get('DETERMINISM_RUNS', '3')),
                       help='Number of compilation runs to compare (default: 3)')
    parser.add_argument('--probability', type=float,
                       default=float(os.environ.get('DETERMINISM_PROBABILITY', '1.0')),
                       help='Probability of checking each test, 0.0-1.0 (default: 1.0)')
    parser.add_argument('-v', '--verbose', action='store_true',
                       help='Verbose output')
    parser.add_argument('--target', 
                       help='Target to test (js, wasm, asmjs, or comma-separated)')
    parser.add_argument('--preexecute', action='store_true',
                       help='Test in preexecution mode')
    parser.add_argument('--params', 
                       help='Additional LIT parameters (e.g., "EXTRA_FLAGS=-O2")')
    
    args = parser.parse_args()
    
    # Validate arguments
    if not args.test_file and not args.suite:
        parser.error("Must specify either test_file or --suite")
    
    if args.test_file and args.suite:
        parser.error("Cannot specify both test_file and --suite")
    
    # Clamp probability
    probability = max(0.0, min(1.0, args.probability))
    
    # Build LIT config parameters
    lit_params = {}
    if args.target:
        lit_params['TARGET'] = args.target
    if args.preexecute:
        lit_params['PRE-EX'] = 'true'
    if args.params:
        for param in args.params.split(','):
            if '=' in param:
                key, value = param.split('=', 1)
                lit_params[key.strip()] = value.strip()
    
    # Collect test files
    test_files = []
    if args.test_file:
        test_files = [args.test_file]
    else:
        test_files = find_test_files(args.suite)
    
    if not test_files:
        print("No test files found")
        return 1
    
    # Run tests
    total = len(test_files)
    passed = 0
    failed = 0
    skipped = 0
    
    print(f"Running {total} tests with determinism checking")
    print(f"  Level: {args.level}")
    print(f"  Runs per test: {args.runs}")
    print(f"  Probability: {probability}")
    if lit_params:
        print(f"  LIT params: {lit_params}")
    print()
    
    for i, test_file in enumerate(test_files):
        # Probability-based sampling
        if not should_test_determinism(probability):
            skipped += 1
            if args.verbose:
                print(f"[{i+1}/{total}] SKIP (probability): {test_file}")
            continue
        
        if args.verbose:
            print(f"[{i+1}/{total}] Testing: {test_file}")
        
        success, message = run_test_with_determinism(
            test_file,
            lit_params,
            args.level,
            args.runs,
            args.verbose
        )
        
        if success:
            passed += 1
            if not args.verbose:
                print(f"[{i+1}/{total}] ✓ {os.path.basename(test_file)}")
            else:
                print(f"  {message}")
        else:
            failed += 1
            print(f"[{i+1}/{total}] ✗ {os.path.basename(test_file)}")
            print(f"  {message}")
    
    # Summary
    print()
    print("="*60)
    print(f"Determinism Test Summary:")
    print(f"  Total:   {total}")
    print(f"  Passed:  {passed}")
    print(f"  Failed:  {failed}")
    print(f"  Skipped: {skipped}")
    print("="*60)
    
    return 0 if failed == 0 else 1

if __name__ == '__main__':
    sys.exit(main())
