#!/bin/bash

# Script to run lit tests with different combinations of flags, targets, and preexecute modes
# Usage: ./run_lit_combinations.sh [test_pattern]
# Example: ./run_lit_combinations.sh test_arithmetic.cpp

set -e

# Configuration
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../jsexport" && pwd)"
OUTPUT_DIR="${SCRIPT_DIR}/test_results_$(date +%Y%m%d_%H%M%S)"
mkdir -p "$OUTPUT_DIR"

TEST_PATTERN="${1:-.}"  # Default to all tests if no pattern provided

# Define test combinations
declare -a TARGETS=(
    "js"
    "wasm"
    "asmjs"
    "js,wasm"
    "js,asmjs"
    "wasm,asmjs"
    "js,wasm,asmjs"
)

declare -a PREEXEC_MODES=(
    "false"
    "true"
)

declare -a FLAG_SETS=(
    ""                                    # No extra flags
    "-O2"                                 # Higher optimization
    "-O3"                                 # Max optimization
    "-cheerp-no-lto"                      # Disable LTO
    "-cheerp-linear-output=wasm"          # Linear output mode
    "-cheerp-pretty-code"                 # Pretty code generation
)

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Summary tracking
TOTAL_RUNS=0
PASSED_RUNS=0
FAILED_RUNS=0

echo "=========================================="
echo "Lit Test Combination Runner"
echo "=========================================="
echo "Output directory: $OUTPUT_DIR"
echo "Test pattern: $TEST_PATTERN"
echo ""

# Run tests with all combinations
for TARGET in "${TARGETS[@]}"; do
    for PREEXEC in "${PREEXEC_MODES[@]}"; do
        for FLAGS in "${FLAG_SETS[@]}"; do
            # Skip invalid flag combinations
            # -cheerp-linear-output=wasm doesn't make sense for JS-only targets
            if [[ "$FLAGS" == *"-cheerp-linear-output=wasm"* ]] && [[ "$TARGET" == "js" ]]; then
                continue
            fi
            
            # Skip -cheerp-no-lto with JS target when test uses test_all_modules.sh
            # (it causes linking issues with module exports)
            if [[ "$FLAGS" == *"-cheerp-no-lto"* ]] && [[ "$TARGET" == *"js"* ]]; then
                # Only skip for tests that use test_all_modules.sh (cat.cpp, cheerp_pimpl_mod.cpp, etc.)
                if [[ "$TEST_PATTERN" =~ (cat|cheerp_pimpl_mod|.*with_driver) ]]; then
                    continue
                fi
            fi
            
            TOTAL_RUNS=$((TOTAL_RUNS + 1))
            
            # Create a sanitized name for the output file
            TARGET_SAFE=$(echo "$TARGET" | tr ',' '_')
            FLAGS_SAFE=$(echo "$FLAGS" | tr ' -' '_' | sed 's/^_//;s/_$//')
            if [ -z "$FLAGS_SAFE" ]; then
                FLAGS_SAFE="no_flags"
            fi
            
            TEST_NAME="target_${TARGET_SAFE}_preexec_${PREEXEC}_flags_${FLAGS_SAFE}"
            OUTPUT_FILE="${OUTPUT_DIR}/${TEST_NAME}.txt"
            
            echo -n "Running: ${TEST_NAME}..."
            
            # Prepare lit command
            LIT_CMD="lit -v"
            LIT_CMD="$LIT_CMD --param=TARGET=\"$TARGET\""
            LIT_CMD="$LIT_CMD --param=PRE-EX=\"$PREEXEC\""
            
            if [ -n "$FLAGS" ]; then
                LIT_CMD="$LIT_CMD --param=CHEERP_FLAGS=\"$FLAGS\""
            fi
            
            LIT_CMD="$LIT_CMD $SCRIPT_DIR/$TEST_PATTERN"
            
            # Write test configuration to output file
            {
                echo "=========================================="
                echo "Test Configuration"
                echo "=========================================="
                echo "Timestamp: $(date)"
                echo "Target(s): $TARGET"
                echo "Preexecute Mode: $PREEXEC"
                echo "Flags: ${FLAGS:-<none>}"
                echo "Test Pattern: $TEST_PATTERN"
                echo "Command: $LIT_CMD"
                echo ""
                echo "=========================================="
                echo "Test Output"
                echo "=========================================="
                echo ""
            } > "$OUTPUT_FILE"
            
            # Run the test and capture output
            if eval "$LIT_CMD" >> "$OUTPUT_FILE" 2>&1; then
                echo -e " ${GREEN}PASSED${NC}"
                PASSED_RUNS=$((PASSED_RUNS + 1))
                echo "RESULT: PASSED" >> "$OUTPUT_FILE"
            else
                echo -e " ${RED}FAILED${NC}"
                FAILED_RUNS=$((FAILED_RUNS + 1))
                echo "RESULT: FAILED" >> "$OUTPUT_FILE"
            fi
        done
    done
done

# Generate summary report
SUMMARY_FILE="${OUTPUT_DIR}/summary.txt"
{
    echo "=========================================="
    echo "Test Run Summary"
    echo "=========================================="
    echo "Generated: $(date)"
    echo ""
    echo "Total Runs: $TOTAL_RUNS"
    echo "Passed: $PASSED_RUNS"
    echo "Failed: $FAILED_RUNS"
    echo "Success Rate: $(awk "BEGIN {printf \"%.2f\", ($PASSED_RUNS/$TOTAL_RUNS)*100}")%"
    echo ""
    echo "=========================================="
    echo "Configuration Details"
    echo "=========================================="
    echo "Targets tested: ${#TARGETS[@]} combinations"
    echo "  - ${TARGETS[*]}"
    echo ""
    echo "Preexecute modes: ${#PREEXEC_MODES[@]}"
    echo "  - ${PREEXEC_MODES[*]}"
    echo ""
    echo "Flag sets: ${#FLAG_SETS[@]}"
    for i in "${!FLAG_SETS[@]}"; do
        echo "  [$i] ${FLAG_SETS[$i]:-<no flags>}"
    done
    echo ""
    echo "=========================================="
    echo "Results by Test"
    echo "=========================================="
    echo ""
    
    # List all results
    for RESULT_FILE in "$OUTPUT_DIR"/*.txt; do
        if [ "$RESULT_FILE" != "$SUMMARY_FILE" ]; then
            FILENAME=$(basename "$RESULT_FILE" .txt)
            if grep -q "RESULT: PASSED" "$RESULT_FILE"; then
                echo "[PASS] $FILENAME"
            else
                echo "[FAIL] $FILENAME"
            fi
        fi
    done
} > "$SUMMARY_FILE"

# Display summary
echo ""
echo "=========================================="
echo "Summary"
echo "=========================================="
cat "$SUMMARY_FILE"
echo ""
echo "All results saved to: $OUTPUT_DIR"
echo "Summary: $SUMMARY_FILE"
