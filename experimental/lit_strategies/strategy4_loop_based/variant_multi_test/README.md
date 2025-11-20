# Variant: Multiple Test Files with Loop Script

This variant demonstrates using the same looping script approach with multiple separate test files.

## Key Features

- **Multiple test files**: Each file tests different functionality
- **Same looping approach**: Uses `%run_all_targets` wrapper script
- **Single test per file**: Each test file runs all targets together

## Structure

```
test_arithmetic.cpp    - Tests basic arithmetic operations
test_modulo.cpp        - Tests modulo and comparison operations
```

## Usage

```bash
# Run all tests for specific targets
lit . --param TARGETS=wasm,js

# Run all tests for all targets
lit .

# Run specific test
lit test_arithmetic.cpp --param TARGETS=wasm
```

## Output Example

```
-- Testing: 2 tests, 2 workers --
PASS: Cheerp-Strategy4 :: test_arithmetic.cpp (1 of 2)
PASS: Cheerp-Strategy4 :: test_modulo.cpp (2 of 2)
```

Each test file runs as a separate lit test, but within each test, all enabled targets are executed via the loop script.

## When to Use

Use this variant when you want to:
- Organize tests by functionality/feature
- Keep each test focused on specific operations
- Still benefit from the loop script running all targets together
- Have clear separation of test concerns
