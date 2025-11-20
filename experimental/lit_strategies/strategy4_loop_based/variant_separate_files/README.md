# Variant: Separate Files Per Target

This variant has completely separate test files for each compilation target, allowing individual execution.

## Key Features

- **One file per target**: `test_wasm.cpp`, `test_js.cpp`, `test_asmjs.cpp`
- **REQUIRES directives**: Each file uses `REQUIRES: <target>` to gate execution
- **Individually callable**: Each test can run independently
- **Separate pass/fail**: Each target gets its own test result

## Structure

```
test_wasm.cpp      - WASM-specific test (REQUIRES: wasm)
test_js.cpp        - JavaScript-specific test (REQUIRES: js)
test_asmjs.cpp     - AsmJS-specific test (REQUIRES: asmjs)
```

## Usage

```bash
# Run tests for specific targets (others show as UNSUPPORTED)
lit . --param TARGETS=wasm,js

# Run all tests for all targets
lit .

# Run a specific target's test
lit test_wasm.cpp --param TARGETS=wasm

# Run just one test file (will skip if target not enabled)
lit test_js.cpp --param TARGETS=wasm  # Shows UNSUPPORTED
```

## Output Example

```
-- Testing: 3 tests, 3 workers --
UNSUPPORTED: Cheerp-Strategy4-Separate :: test_asmjs.cpp (1 of 3)
PASS: Cheerp-Strategy4-Separate :: test_js.cpp (2 of 3)
PASS: Cheerp-Strategy4-Separate :: test_wasm.cpp (3 of 3)
```

## When to Use

Use this variant when you want to:
- See explicit pass/fail for each target separately
- Run tests for specific targets only
- Have complete separation between target tests
- Debug or develop for one target at a time
- Want clear visibility into which specific target passed/failed

## Advantages Over Loop Script

- ✅ Each target shows as a separate test result
- ✅ Can run individual target tests
- ✅ Clear which target failed
- ✅ No dependencies between target tests

## Trade-offs

- ❌ Code duplication across test files
- ❌ Must update all files if test logic changes
- ⚠️ UNSUPPORTED tests shown when targets are disabled (expected behavior)
