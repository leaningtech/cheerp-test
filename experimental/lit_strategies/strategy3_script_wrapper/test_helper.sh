#!/bin/bash
# Test helper script for Strategy 3
# This script handles conditional compilation and testing based on enabled targets

set -e

# Parse arguments
MODE=""
TARGETS=""
OUTPUT=""
SOURCE=""
CHECK_TYPE=""

while [[ $# -gt 0 ]]; do
    case $1 in
        --compile)
            MODE="compile"
            TARGETS="$2"
            shift 2
            ;;
        --verify)
            MODE="verify"
            TARGETS="$2"
            shift 2
            ;;
        --execute)
            MODE="execute"
            TARGETS="$2"
            shift 2
            ;;
        --output)
            OUTPUT="$2"
            shift 2
            ;;
        --source)
            SOURCE="$2"
            shift 2
            ;;
        --check-magic)
            CHECK_TYPE="magic"
            shift
            ;;
        --check-loader)
            CHECK_TYPE="loader"
            shift
            ;;
        --check-output)
            CHECK_TYPE="output"
            shift
            ;;
        *)
            echo "Unknown option: $1"
            exit 1
            ;;
    esac
done

# Helper to check if target is enabled
is_target_enabled() {
    local target=$1
    case $target in
        wasm)
            [[ "${HAS_WASM:-1}" == "1" ]]
            ;;
        js)
            [[ "${HAS_JS:-1}" == "1" ]]
            ;;
        asmjs)
            [[ "${HAS_ASMJS:-1}" == "1" ]]
            ;;
        *)
            return 1
            ;;
    esac
}

# Compile mode
if [[ "$MODE" == "compile" ]]; then
    IFS=',' read -ra TARGET_ARRAY <<< "$TARGETS"
    for target in "${TARGET_ARRAY[@]}"; do
        target=$(echo "$target" | xargs)  # trim whitespace
        if is_target_enabled "$target"; then
            case $target in
                wasm)
                    ${CHEERP_CLANG:-/opt/cheerp/bin/clang++} -target cheerp-wasm -o "$OUTPUT/w" "$SOURCE"
                    ;;
                js)
                    ${CHEERP_CLANG:-/opt/cheerp/bin/clang++} -target cheerp -o "$OUTPUT/j" "$SOURCE"
                    ;;
                asmjs)
                    ${CHEERP_CLANG:-/opt/cheerp/bin/clang++} -target cheerp-asmjs -o "$OUTPUT/a" "$SOURCE"
                    ;;
            esac
        fi
    done
    exit 0
fi

# Verify mode
if [[ "$MODE" == "verify" ]]; then
    if ! is_target_enabled "$TARGETS"; then
        exit 0  # Skip if target not enabled
    fi
    
    case $TARGETS in
        wasm)
            case $CHECK_TYPE in
                magic)
                    test -f "$OUTPUT/w.wasm" && test -f "$OUTPUT/w"
                    xxd -l 8 "$OUTPUT/w.wasm" | grep "0061 736d"
                    ;;
                loader)
                    grep -q "WebAssembly" "$OUTPUT/w"
                    ;;
            esac
            ;;
        js)
            case $CHECK_TYPE in
                output)
                    test -f "$OUTPUT/j"
                    grep -qE "(printf|console)" "$OUTPUT/j"
                    ;;
            esac
            ;;
    esac
    exit 0
fi

# Execute mode
if [[ "$MODE" == "execute" ]]; then
    IFS=',' read -ra TARGET_ARRAY <<< "$TARGETS"
    for target in "${TARGET_ARRAY[@]}"; do
        target=$(echo "$target" | xargs)  # trim whitespace
        if is_target_enabled "$target"; then
            case $target in
                wasm)
                    node "$OUTPUT/w" 2>&1
                    ;;
                js)
                    node "$OUTPUT/j" 2>&1
                    ;;
                asmjs)
                    node "$OUTPUT/a" 2>&1
                    ;;
            esac
        fi
    done
    exit 0
fi
