#!/bin/bash
# Script to test if we can catch compilation bugs by comparing outputs

set -e

TEST_DIR="/home/alex/cheerp/tests/unit/memory"
OUTPUT_DIR="$TEST_DIR/Output"
CHEERP_CLANG="/opt/cheerp/bin/clang++"

echo "=== Testing Compilation Bug Detection ==="

# Step 1: Compile the reference (correct) version 
echo "Step 1: Compiling reference multiarray.cpp..."
$CHEERP_CLANG -O1 -frtti -I$TEST_DIR/.. \
    -cheerp-bounds-check -cheerp-fix-wrong-func-casts \
    -target cheerp \
    $TEST_DIR/multiarray.cpp \
    -o $OUTPUT_DIR/multiarray_reference.js

# Step 2: Run reference and capture output
echo "Step 2: Running reference version..."
node $OUTPUT_DIR/multiarray_reference.js 2>&1 > $OUTPUT_DIR/reference_output.txt
echo "Reference output:"
cat $OUTPUT_DIR/reference_output.txt

# Step 3: Compile a version we'll corrupt
echo ""
echo "Step 3: Compiling version to corrupt..."
$CHEERP_CLANG -O1 -frtti -I$TEST_DIR/.. \
    -cheerp-bounds-check -cheerp-fix-wrong-func-casts \
    -target cheerp \
    $TEST_DIR/multiarray.cpp \
    -o $OUTPUT_DIR/multiarray_to_corrupt.js

# Step 4: Make a copy to manually corrupt
cp $OUTPUT_DIR/multiarray_to_corrupt.js $OUTPUT_DIR/multiarray_corrupted.js

echo ""
echo "Step 4: Now you can manually edit: $OUTPUT_DIR/multiarray_corrupted.js"
echo "Examples of corruptions to introduce:"
echo "  - Change array indexing calculations"
echo "  - Modify constructor count increments"
echo "  - Alter arithmetic operations"
echo ""
echo "After editing, press Enter to continue..."
read

# Step 5: Run corrupted version and capture output
echo "Step 5: Running corrupted version..."
node $OUTPUT_DIR/multiarray_corrupted.js 2>&1 > $OUTPUT_DIR/corrupted_output.txt
echo "Corrupted output:"
cat $OUTPUT_DIR/corrupted_output.txt

# Step 6: Compare outputs
echo ""
echo "Step 6: Comparing outputs..."
if diff $OUTPUT_DIR/reference_output.txt $OUTPUT_DIR/corrupted_output.txt > $OUTPUT_DIR/diff_result.txt 2>&1; then
    echo "❌ TEST FAILED: No difference detected (test did not catch the bug!)"
    exit 1
else
    echo "✅ TEST PASSED: Difference detected!"
    echo ""
    echo "Differences found:"
    cat $OUTPUT_DIR/diff_result.txt
    exit 0
fi
