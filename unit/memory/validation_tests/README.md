# Memory Validation Tests

This directory contains systematic validation tests for all memory-related tests.

## Structure

For each test file `testX.cpp` in the parent directory, this folder contains:

- `testX_bugless_test.cpp` - Verifies identical compilations produce identical output
- `testX_corruption_test.cpp` - Verifies the test framework catches compilation bugs

## Purpose

- **Bugless tests**: Ensure test framework baseline correctness
- **Corruption tests**: Ensure test framework catches compilation bugs

## Running Tests

```bash
# Run all validation tests
lit unit/memory/validation_tests/

# Run specific test type
lit unit/memory/validation_tests/*_bugless_test.cpp
lit unit/memory/validation_tests/*_corruption_test.cpp
```

## Expected Results

- **All tests should PASS**
- Bugless tests verify identical outputs
- Corruption tests verify different outputs are detected
