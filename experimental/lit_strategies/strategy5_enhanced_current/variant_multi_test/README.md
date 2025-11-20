# Variant: Multiple Test Files with Strategy 5 Enhanced Macros

This variant demonstrates using the enhanced macro approach from Strategy 5 with multiple separate test files.

## Key Features

- **Multiple test files**: Each file tests different functionality
- **Enhanced macro syntax**: Uses `%compile_for{target}` and `%run_if{target}` macros
- **Clean test structure**: Each test file is self-contained and explicit about target handling
- **Familiar patterns**: Similar to the original approach but with much cleaner syntax

## Structure

```
test_arithmetic.cpp    - Tests basic arithmetic operations (all targets)
test_modulo.cpp        - Tests modulo and comparison operations (all targets)
lit.cfg                - Configuration file with macro definitions
README.md              - This file
```

## Test Syntax Comparison

### Original (verbose)
```cpp
// RUN: if [ "$HAS_WASM" = "1" ]; then %cheerp_clang -target cheerp-wasm -o %t/w %s; fi
// RUN: if [ "$HAS_WASM" = "1" ]; then %node %t/w 2>&1 | %FileCheck %s; fi
```

### Strategy 5 (clean)
```cpp
// RUN: %compile_for{wasm} -O1 -o %t/w %s
// RUN: %run_if{wasm} %node %t/w 2>&1 | %FileCheck %s
```

## Usage

### Run all tests for specific targets
```bash
lit . --param TARGETS=wasm,js
```

### Run all tests for all available targets
```bash
lit .
```

### Run specific test
```bash
lit test_arithmetic.cpp --param TARGETS=wasm
```

### Run with verbose output
```bash
lit . -v
```

## Output Example

```
-- Testing: 2 tests, 1 worker --
PASS: Cheerp-Strategy5-MultiTest :: test_arithmetic.cpp (1 of 2)
PASS: Cheerp-Strategy5-MultiTest :: test_modulo.cpp (2 of 2)
===================== 2 passed in 0.45s =====================
```

Each test file runs independently, with all enabled targets executed within that test.

## How the Macros Work

### %compile_for{target}
- **When enabled**: Expands to full compiler command with target flags
- **When disabled**: Replaced with `true` (no-op)
- **Example**: `%compile_for{wasm}` → `/opt/cheerp/bin/clang++ -target cheerp-wasm`

### %run_if{target}
- **When enabled**: Prefix is stripped, command runs normally
- **When disabled**: Entire line replaced with `true` (no-op)
- **Example**: `%run_if{wasm} %node %t/w` → `%node %t/w` (when WASM enabled)

## Differences from Strategy 4

| Aspect | Strategy 4 (Loop-based) | Strategy 5 (Enhanced Current) |
|--------|------------------------|------------------------------|
| **Test execution** | Single wrapped command per file | Individual RUN lines per target |
| **Verbosity** | Very low (6-7 lines) | Low-Medium (15-18 lines) |
| **Duplication** | Minimal—test logic appears once | Moderate—target names repeated |
| **Target reporting** | Could show separate target results | All results in one test output |
| **Complexity** | Requires wrapper script | Pure lit substitutions |
| **Migration ease** | Medium | Very easy (search/replace) |

## Advantages of This Variant

1. **Explicit control**: Each target's compilation and execution is clearly visible
2. **Easy debugging**: Can disable specific targets for troubleshooting
3. **Familiar patterns**: Developers used to lit's RUN syntax feel at home
4. **No external scripts**: Everything defined in lit.cfg
5. **Incremental adoption**: Can migrate existing tests one at a time

## When to Use

Use Strategy 5 when you want to:
- Migrate from the original approach with minimal refactoring
- Keep test files readable and explicit about target handling
- Avoid external helper scripts
- Maintain clear separation between different targets in the test structure
- Use a drop-in replacement for existing conditional compilation tests

## Environment Variables

- `CHEERP_FLAGS` - Additional compiler flags applied to all compilations
- `EXTRA_FLAGS` - Extra flags appended to all compilations
- `TARGETS` - Comma-separated list of targets (js, wasm, asmjs)

Example:
```bash
CHEERP_FLAGS="-O2" lit . --param TARGETS=wasm,js
```

## Troubleshooting

### Test fails with "command not found"
- Ensure `/opt/cheerp/bin/clang++` exists or update `cheerp_base` in `lit.cfg`
- Check that `/usr/bin/FileCheck-20` is installed

### All %run_if lines become "true"
- This is expected when running without specifying TARGETS
- The default selects all available targets (js, wasm, asmjs)
- If a target isn't installed, lines for it will become `true`

### One target works but others don't
- Check the FileCheck output patterns use appropriate prefixes
- Verify each target produces the expected output format
- Use `lit . -v` for verbose output to see actual commands being run
