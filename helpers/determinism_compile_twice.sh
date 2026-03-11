#!/bin/bash
# Determinism compile twice script
# Usage: determinism_compile_twice.sh <compiler_cmd> <output_base> <source_file>
#
# This script:
# 1. Compiles the source file twice with the same compiler command
# 2. Compares the outputs bit-for-bit
# 3. Outputs "DETERMINISTIC: <filename>" to stdout for each matching file
# 4. Outputs "NON-DETERMINISTIC: <filename>" to stdout for files that differ
#
# FileCheck can then verify these outputs

COMPILER_CMD="$1"
OUTPUT_BASE="$2"
SOURCE_FILE="$3"

# Create output directories
RUN0_DIR="${OUTPUT_BASE}/run_0"
RUN1_DIR="${OUTPUT_BASE}/run_1"
mkdir -p "$RUN0_DIR" "$RUN1_DIR"

# Compile first time (stderr only, no stdout)
eval "$COMPILER_CMD -o $RUN0_DIR/output $SOURCE_FILE" 2>&1 > /dev/null

# Compile second time (stderr only, no stdout)
eval "$COMPILER_CMD -o $RUN1_DIR/output $SOURCE_FILE" 2>&1 > /dev/null

# Compare all generated files
for file in "$RUN0_DIR"/*; do
    if [ -f "$file" ]; then
        filename=$(basename "$file")
        file1="$RUN0_DIR/$filename"
        file2="$RUN1_DIR/$filename"
        
        if [ ! -f "$file2" ]; then
            echo "NON-DETERMINISTIC: $filename (missing in run_1)"
            continue
        fi
        
        # Compare files bit-for-bit
        if cmp -s "$file1" "$file2"; then
            echo "DETERMINISTIC: $filename"
        else
            echo "NON-DETERMINISTIC: $filename"
        fi
    fi
done

# Check for files that exist in run_1 but not run_0
for file in "$RUN1_DIR"/*; do
    if [ -f "$file" ]; then
        filename=$(basename "$file")
        file0="$RUN0_DIR/$filename"
        
        if [ ! -f "$file0" ]; then
            echo "NON-DETERMINISTIC: $filename (missing in run_0)"
        fi
    fi
done