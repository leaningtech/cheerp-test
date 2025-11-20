#!/usr/bin/env python3
"""
Generate separate test files for each target (js, wasm, asmjs) from jsexport tests.
"""

import os
import shutil

# Source and destination directories
JSEXPORT_DIR = "/home/alex/cheerp/cheerp-test/unit/jsexport"
DEST_DIR = "/home/alex/cheerp/cheerp-test/experimental/lit_strategies/strategy4_loop_based/variant_separate_files"

# Targets to generate
TARGETS = ["js", "wasm", "asmjs"]

# Test files that have .testing.js files
TEST_FILES = [
    "array_of_structs",
    "cat",
    "cheerp_pimpl_mod",
    "compare_pointers",
    "empty_class",
    "global_variables",
    "inheritance",
    "member_variables",
    "nested_variables",
    "static_variables",
]

def read_file(filepath):
    """Read file contents."""
    with open(filepath, 'r') as f:
        return f.read()

def write_file(filepath, content):
    """Write content to file."""
    with open(filepath, 'w') as f:
        f.write(content)

def create_target_test(test_name, target):
    """Create a separate test file for a specific target."""
    cpp_file = os.path.join(JSEXPORT_DIR, f"{test_name}.cpp")
    testing_file = os.path.join(JSEXPORT_DIR, f"{test_name}.testing.js")
    
    # Read the original cpp file
    cpp_content = read_file(cpp_file)
    
    # Extract the main code (skip original RUN lines)
    lines = cpp_content.split('\n')
    code_lines = []
    in_run_section = True
    
    for line in lines:
        if in_run_section:
            if line.startswith('//') and 'RUN:' not in line and 'CHECK' not in line:
                in_run_section = False
                code_lines.append(line)
            elif not line.startswith('//'):
                in_run_section = False
                code_lines.append(line)
        else:
            code_lines.append(line)
    
    # Create new RUN lines for this specific target
    header = f"// Test {test_name} for {target} target\n"
    header += f"// REQUIRES: {target}\n"
    header += f"// RUN: mkdir -p %t\n"
    
    if target == "wasm":
        header += f"// RUN: %cheerp_clang -target cheerp-wasm -o %t/out %s\n"
    elif target == "asmjs":
        header += f"// RUN: %cheerp_clang -target cheerp-asmjs -o %t/out %s\n"
    else:  # js
        header += f"// RUN: %cheerp_clang -target cheerp -o %t/out %s\n"
    
    # Add node execution if there's a testing file
    if os.path.exists(testing_file):
        # Need to create driver file
        header += f"// RUN: cp %S/webworker-polyfill.js %t/ 2>/dev/null || true\n"
        header += f"// RUN: cat %t/out > %t/driver.js\n"
        header += f"// RUN: cat %S/import-polyfills.js >> %t/driver.js 2>/dev/null || true\n"
        header += f"// RUN: cat %S/{test_name}.testing.js >> %t/driver.js\n"
        header += f"// RUN: echo 'var EXPORTS = getExports()' >> %t/driver.js\n"
        header += f"// RUN: echo 'getPromise(EXPORTS).then(_=>{{onInstantiation(getExports())}}).catch(err => {{onError(err)}})' >> %t/driver.js\n"
        header += f"// RUN: %node %t/driver.js 2>&1\n"
    else:
        header += f"// RUN: %node %t/out 2>&1\n"
    
    # Combine header and code
    new_content = header + '\n'.join(code_lines)
    
    # Write to new file
    output_file = os.path.join(DEST_DIR, f"{test_name}_{target}.cpp")
    write_file(output_file, new_content)
    
    # Copy testing file if it exists
    if os.path.exists(testing_file):
        dest_testing = os.path.join(DEST_DIR, f"{test_name}.testing.js")
        if not os.path.exists(dest_testing):  # Only copy once
            shutil.copy(testing_file, dest_testing)
    
    print(f"Created {output_file}")

def main():
    """Main function to generate all test files."""
    for test_name in TEST_FILES:
        for target in TARGETS:
            create_target_test(test_name, target)
    
    print(f"\nGenerated {len(TEST_FILES) * len(TARGETS)} test files")

if __name__ == "__main__":
    main()
