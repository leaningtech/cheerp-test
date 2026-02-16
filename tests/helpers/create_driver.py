#!/usr/bin/env python3
"""
Helper script to create JS driver files for running Cheerp jsexport tests with .testing.js files.
This mimics the behavior from cheerp-utils/tests/run_tests.py but for lit tests.

Usage:
  create_driver.py <compiled_js_file> <testing_js_file> <output_driver_file> [--module=<type>] [--pthread]

Module types: vanilla (default), es6, commonjs, closure
"""

import sys
import os
import argparse

def create_driver(compiled_file, testing_file, output_file, module='vanilla', pthread=False, polyfill_file=None):
    
    # Try to find polyfill file in standard locations if not provided
    if not polyfill_file:
        script_dir = os.path.dirname(os.path.abspath(__file__))
        # Search a few common relative locations for import-polyfills.js.
        #
        # Layouts we support:
        #   - cheerp-test/stable/js-helpers/import-polyfills.js
        #   - cheerp-test/test-wrappers/import-polyfills.js
        #
        # Note: this script lives in stable/helpers/, so we need to walk up
        # one or two directories to reach those locations.
        possible_paths = [
            # stable/helpers/js-helpers/import-polyfills.js (legacy expectation)
            os.path.join(script_dir, 'js-helpers/import-polyfills.js'),
            # stable/js-helpers/import-polyfills.js (current layout)
            os.path.join(script_dir, '../js-helpers/import-polyfills.js'),
            # stable/test-wrappers/import-polyfills.js (legacy expectation)
            os.path.join(script_dir, '../test-wrappers/import-polyfills.js'),
            # test-wrappers/import-polyfills.js at repo root
            os.path.join(script_dir, '../../test-wrappers/import-polyfills.js'),
        ]
        for path in possible_paths:
            if os.path.exists(path):
                polyfill_file = path
                break
    
    # Default polyfill content (if no file found)
    polyfill_content = """
globalThis.Blob = require('node:buffer').Blob;
""".strip()
    
    # Read polyfill file and adjust paths to be relative to output directory
    if polyfill_file and os.path.exists(polyfill_file):
        with open(polyfill_file, 'r') as f:
            raw_content = f.read().strip()
        
        # Get directories for relative path calculation
        output_dir = os.path.dirname(os.path.abspath(output_file))
        polyfill_dir = os.path.dirname(os.path.abspath(polyfill_file))
        
        # Adjust relative paths in polyfill content to be relative to output file
        polyfill_content = raw_content
        
        # If polyfill references './webworker-polyfill.js', make it relative to output
        if './webworker-polyfill.js' in polyfill_content:
            webworker_path = os.path.join(polyfill_dir, 'webworker-polyfill.js')
            if os.path.exists(webworker_path):
                rel_path = os.path.relpath(webworker_path, output_dir)
                polyfill_content = polyfill_content.replace(
                    "require('./webworker-polyfill.js')",
                    f"require('{rel_path}')"
                )
    
    # Read testing file
    with open(testing_file, 'r') as f:
        testing_content = f.read()
    
    # Get base name for module imports
    base_name = os.path.splitext(os.path.basename(compiled_file))[0]
    base_dir = os.path.dirname(compiled_file)
    
    with open(output_file, 'w') as out:
        if module == 'es6':
            # ES6 module driver
            out.write(f"import module from './{base_name}.mjs'\n")
            out.write("import { createRequire } from 'node:module';\n")
            out.write("const require = createRequire(import.meta.url);\n")
            out.write(polyfill_content + "\n")
            out.write(testing_content + "\n")
            out.write("module({relativePath:'', argv: typeof argv == 'undefined' ? [] : argv, env: typeof env == 'undefined' ? [] : env})")
            out.write(".then(_ => {onInstantiation(_)}).catch(err => {onError(err)})\n")
        
        elif module == 'commonjs':
            # CommonJS module driver
            out.write(polyfill_content + "\n")
            out.write(testing_content + "\n")
            out.write(f"require('./{base_name}').then(_ => {{onInstantiation(_)}}).catch(err => {{onError(err)}})\n")
        
        elif module == 'closure':
            # Closure module driver - inline the compiled code
            with open(compiled_file, 'r') as f:
                compiled_content = f.read()
            out.write(compiled_content + "\n")
            if pthread:
                out.write("if (typeof require==='function'){\n")
            out.write(polyfill_content + "\n")
            out.write(testing_content + "\n")
            out.write("getPromise(global).then(_=>{onInstantiation(global)}).catch(err => {onError(err)})\n")
            if pthread:
                out.write("}\n")
        
        else:  # vanilla (default)
            # Vanilla/no module driver - inline the compiled code
            with open(compiled_file, 'r') as f:
                compiled_content = f.read()
            out.write(compiled_content + "\n")
            if pthread:
                out.write("if (typeof require === 'function'){\n")
            out.write(polyfill_content + "\n")
            out.write(testing_content + "\n")
            out.write("var EXPORTS = getExports()\n")
            out.write("getPromise(EXPORTS).then(_=>{onInstantiation(getExports())}).catch(err => {onError(err)})\n")
            if pthread:
                out.write("}\n")

def main():
    parser = argparse.ArgumentParser(description='Create JS driver file for Cheerp jsexport tests')
    parser.add_argument('compiled_file', help='Path to compiled JS file')
    parser.add_argument('testing_file', help='Path to .testing.js file')
    parser.add_argument('output_file', help='Path to output driver file')
    parser.add_argument('--module', choices=['vanilla', 'es6', 'commonjs', 'closure'], 
                       default='vanilla', help='Module type')
    parser.add_argument('--pthread', action='store_true', help='Enable pthread support')
    parser.add_argument('--polyfill', help='Path to polyfill file (optional)')
    
    args = parser.parse_args()
    
    if not os.path.exists(args.compiled_file):
        print(f"Error: Compiled file not found: {args.compiled_file}", file=sys.stderr)
        sys.exit(1)
    
    if not os.path.exists(args.testing_file):
        print(f"Error: Testing file not found: {args.testing_file}", file=sys.stderr)
        sys.exit(1)
    
    create_driver(args.compiled_file, args.testing_file, args.output_file, 
                 args.module, args.pthread, args.polyfill)
    print(f"Created driver: {args.output_file}")

if __name__ == '__main__':
    main()
