#!/bin/bash
# Test a jsexport test with all 4 module types (vanilla, ES6, CommonJS, closure)
#
# Usage: test_all_modules.sh <source.cpp> <testing.js> <output_dir> <compiler_base_cmd>
#
# Example:
#   test_all_modules.sh mytest.cpp mytest.testing.js /tmp/test "clang++ -target cheerp -O1"
#
# Note: This script is designed for regular (non-preexec) mode testing only.
#       For preexec mode, tests should be run directly without .testing.js drivers.

set -e

if [ $# -lt 4 ]; then
    echo "Usage: $0 <source.cpp> <testing.js> <output_dir> <compiler_base_cmd>"
    exit 1
fi

SOURCE="$1"
TESTING_JS="$2"
OUTPUT_DIR="$3"
shift 3
COMPILER_CMD="$@"

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
CREATE_DRIVER="$SCRIPT_DIR/create_driver.py"

# Check if testing.js exists
if [ ! -f "$TESTING_JS" ]; then
    echo "Error: Testing file not found: $TESTING_JS"
    exit 1
fi

mkdir -p "$OUTPUT_DIR"

# Test 1: Vanilla (no module)
echo "Testing vanilla module..."
$COMPILER_CMD -o "$OUTPUT_DIR/vanilla.js" "$SOURCE"
python3 "$CREATE_DRIVER" "$OUTPUT_DIR/vanilla.js" "$TESTING_JS" "$OUTPUT_DIR/vanilla_driver.js" --module=vanilla
node "$OUTPUT_DIR/vanilla_driver.js"

# Test 2: ES6 module
echo "Testing ES6 module..."
$COMPILER_CMD -cheerp-make-module=es6 -o "$OUTPUT_DIR/es6.mjs" "$SOURCE"
python3 "$CREATE_DRIVER" "$OUTPUT_DIR/es6.mjs" "$TESTING_JS" "$OUTPUT_DIR/es6_driver.mjs" --module=es6
node "$OUTPUT_DIR/es6_driver.mjs"

# Test 3: CommonJS module
echo "Testing CommonJS module..."
$COMPILER_CMD -cheerp-make-module=commonjs -o "$OUTPUT_DIR/commonjs.js" "$SOURCE"
python3 "$CREATE_DRIVER" "$OUTPUT_DIR/commonjs.js" "$TESTING_JS" "$OUTPUT_DIR/commonjs_driver.js" --module=commonjs
node "$OUTPUT_DIR/commonjs_driver.js"

# Test 4: Closure module
echo "Testing Closure module..."
$COMPILER_CMD -cheerp-make-module=closure -o "$OUTPUT_DIR/closure.js" "$SOURCE"
python3 "$CREATE_DRIVER" "$OUTPUT_DIR/closure.js" "$TESTING_JS" "$OUTPUT_DIR/closure_driver.js" --module=closure
node "$OUTPUT_DIR/closure_driver.js"

echo "All module types passed!"
