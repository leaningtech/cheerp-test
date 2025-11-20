#!/bin/bash
# Wrapper script that runs tests once per enabled target
# This allows each target to have separate pass/fail reporting

set -e

SOURCE_FILE="$1"
OUTPUT_DIR="$2"

# Get the base name without extension
BASE_NAME=$(basename "$SOURCE_FILE" .cpp)
BASE_NAME=$(basename "$BASE_NAME" .c)
SOURCE_DIR=$(dirname "$SOURCE_FILE")
TESTING_FILE="${SOURCE_DIR}/${BASE_NAME}.testing.js"

IFS=',' read -ra TARGETS <<< "$SELECTED_TARGETS"

for TARGET in "${TARGETS[@]}"; do
    case "$TARGET" in
        wasm)
            TARGET_FLAG="-target cheerp-wasm"
            TARGET_SHORT="w"
            ;;
        js)
            TARGET_FLAG="-target cheerp"
            TARGET_SHORT="j"
            ;;
        asmjs)
            TARGET_FLAG="-target cheerp-asmjs"
            TARGET_SHORT="a"
            ;;
        *)
            echo "Unknown target: $TARGET"
            exit 1
            ;;
    esac
    
    echo "=== Testing target: $TARGET ==="
    
    # Compile with required flags
    $CHEERP_CLANG -O1 -frtti -I"$SOURCE_DIR" -cheerp-bounds-check -cheerp-fix-wrong-func-casts $TARGET_FLAG -o "$OUTPUT_DIR/$TARGET_SHORT" "$SOURCE_FILE"
    
    # Verify
    if [ "$TARGET" = "wasm" ]; then
        test -f "$OUTPUT_DIR/$TARGET_SHORT" || exit 1
        test -f "$OUTPUT_DIR/$TARGET_SHORT.wasm" || exit 1
    else
        test -f "$OUTPUT_DIR/$TARGET_SHORT" || exit 1
    fi
    
    # Check if .testing.js file exists and create driver if needed
    if [ -f "$TESTING_FILE" ]; then
        DRIVER_FILE="${OUTPUT_DIR}/${TARGET_SHORT}.vanilla.js"
        POLYFILL_FILE="$(dirname "$0")/import-polyfills.js"
        WEBWORKER_FILE="$(dirname "$0")/webworker-polyfill.js"
        
        # Copy webworker polyfill to output dir so require can find it
        if [ -f "$WEBWORKER_FILE" ]; then
            cp "$WEBWORKER_FILE" "$OUTPUT_DIR/"
        fi
        
        # Create vanilla driver file (default, no module system)
        cat "$OUTPUT_DIR/$TARGET_SHORT" > "$DRIVER_FILE"
        if [ -f "$POLYFILL_FILE" ]; then
            cat "$POLYFILL_FILE" >> "$DRIVER_FILE"
        fi
        cat "$TESTING_FILE" >> "$DRIVER_FILE"
        echo "var EXPORTS = getExports()" >> "$DRIVER_FILE"
        echo "getPromise(EXPORTS).then(_=>{onInstantiation(getExports())}).catch(err => {onError(err)})" >> "$DRIVER_FILE"
        
        # Execute driver file
        node "$DRIVER_FILE" 2>&1
    else
        # No testing file, execute compiled output directly
        node "$OUTPUT_DIR/$TARGET_SHORT" 2>&1
    fi
done
