#!/bin/bash
# Wrapper script that runs tests once per enabled target
# This allows each target to have separate pass/fail reporting

set -e

SOURCE_FILE="$1"
OUTPUT_DIR="$2"

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
    
    # Compile
    $CHEERP_CLANG $TARGET_FLAG -o "$OUTPUT_DIR/$TARGET_SHORT" "$SOURCE_FILE"
    
    # Verify
    if [ "$TARGET" = "wasm" ]; then
        test -f "$OUTPUT_DIR/$TARGET_SHORT" || exit 1
        test -f "$OUTPUT_DIR/$TARGET_SHORT.wasm" || exit 1
    else
        test -f "$OUTPUT_DIR/$TARGET_SHORT" || exit 1
    fi
    
    # Execute and capture output
    node "$OUTPUT_DIR/$TARGET_SHORT" 2>&1
done
