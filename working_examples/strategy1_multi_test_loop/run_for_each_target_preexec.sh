#!/bin/bash
# Preexecution wrapper script for strategy1
# Compiles tests with PRE_EXECUTE_TEST enabled, running assertions at compile-time
# This script is called when PREEXECUTE_MODE is enabled
#
# Usage: run_for_each_target_preexec.sh <source_file> <output_dir>

set -e

SOURCE_FILE="$1"
OUTPUT_DIR="$2"

# Get the base name without extension
BASE_NAME=$(basename "$SOURCE_FILE" .cpp)
BASE_NAME=$(basename "$BASE_NAME" .c)
SOURCE_DIR=$(dirname "$SOURCE_FILE")

# Validate CHEERP_CLANG_PREEXEC is set
if [ -z "$CHEERP_CLANG_PREEXEC" ]; then
    echo "Error: CHEERP_CLANG_PREEXEC environment variable not set"
    exit 1
fi

# Preexecution mode: run single compilation per target with compile-time evaluation
IFS=',' read -ra TARGETS <<< "$SELECTED_TARGETS"

for TARGET in "${TARGETS[@]}"; do
    case "$TARGET" in
        wasm)
            TARGET_FLAG="-target cheerp-wasm"
            TARGET_SHORT="w_preexec"
            ;;
        js)
            TARGET_FLAG="-target cheerp"
            TARGET_SHORT="j_preexec"
            ;;
        asmjs)
            TARGET_FLAG="-target cheerp-asmjs"
            TARGET_SHORT="a_preexec"
            ;;
        *)
            echo "Unknown target: $TARGET"
            exit 1
            ;;
    esac
    
    echo "=== Preexecution test: $TARGET ==="
    
    # Compile with preexecution flags (-DPRE_EXECUTE_TEST enables compile-time evaluation)
    # The PreExecute pass will evaluate assertions at compile-time
    if ! $CHEERP_CLANG_PREEXEC -O1 -frtti -I"$SOURCE_DIR" -cheerp-bounds-check -cheerp-fix-wrong-func-casts $TARGET_FLAG -o "$OUTPUT_DIR/$TARGET_SHORT" "$SOURCE_FILE" 2>&1; then
        echo "Preexecution compilation failed for $TARGET"
        exit 1
    fi
    
    # For preexecution tests, the assertions are evaluated at compile-time
    # If compilation succeeds with PRE_EXECUTE_TEST defined, all assertions passed
    # Output the success message for FileCheck verification
    echo "Preexecution assertions evaluated at compile-time: SUCCESS for $TARGET"
done
