# Test Wrappers for XFAIL Tests

This directory contains JavaScript wrapper scripts that enable XFAIL tests to run in the lit test framework.

## Purpose

Some Cheerp tests require special setup that the original `run_tests.py` provided:
- ES6/CommonJS module loading
- Setting up `argv` and `env` for `main(int argc, char** argv)`
- Calling `webMain()` functions
- Threading/pthread infrastructure
- Exception handling

These wrappers bridge the gap between lit's simple test model and Cheerp's more complex test requirements.

## Wrapper Types

### 1. CLI Wrappers (argv/env setup)
- `cli-argv-wrapper.mjs` - Sets up command-line arguments for tests using `main()`
- `cli-env-wrapper.mjs` - Sets up environment variables

**Usage in test file:**
```cpp
// RUN: %cheerp_clang ... -cheerp-make-module=es6 %s -o %t.mjs
// RUN: %S/../../test-wrappers/cli-argv-wrapper.mjs %t.mjs 2>&1 | %FileCheck %s
```

### 2. Generic ES6 Module Wrapper
For tests that just need ES6 module loading without special argv/env:
```cpp
// RUN: %cheerp_clang ... -cheerp-make-module=es6 %s -o %t.mjs
// RUN: %S/../../test-wrappers/generic-es6-wrapper.mjs %t.mjs 2>&1 | %FileCheck %s
```

### 3. WebMain Wrapper
For tests using `void webMain()` instead of `main()`:
```cpp
// RUN: %cheerp_clang ... -cheerp-make-module=es6 %s -o %t.mjs
// RUN: %S/../../test-wrappers/webmain-wrapper.mjs %t.mjs 2>&1 | %FileCheck %s
```

## Creating New Wrappers

When creating a wrapper for a new test:

1. **Identify what the test needs:**
   - Check if it has `.testing.js` in cheerp-utils
   - Look at the function signature (`main()` vs `webMain()`)
   - Check what environment setup it expects

2. **Copy an existing wrapper** as a template

3. **Update the test file:**
   - Remove `// XFAIL: *` line
   - Change compilation to use `-cheerp-make-module=es6` and output to `.mjs`
   - Update RUN line to use the wrapper
   - Ensure CHECK patterns match the actual output

4. **Test it:**
   ```bash
   lit -v unit/path/to/test.cpp
   ```

## Example: Converting an XFAIL Test

**Before (XFAIL):**
```cpp
// XFAIL: *
// NOTE: Requires .testing.js harness
// RUN: %cheerp_clang -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
```

**After (with wrapper):**
```cpp
// RUN: %cheerp_clang -target cheerp -cheerp-make-module=es6 %s -o %t.mjs
// RUN: %S/../../test-wrappers/cli-argv-wrapper.mjs %t.mjs 2>&1 | %FileCheck %s
```

## Testing Status

**Working with wrappers:**
- ✅ `unit/cli/argv.cpp` - Uses `cli-argv-wrapper.mjs`
- ⚠️ `unit/cli/env.cpp` - Wrapper created, needs testing
- 🔄 15 more XFAIL tests remaining

**Still need wrappers:**
- Tests with `webMain()` (7 tests)
- Threading tests with `-pthread` (5 tests)
- Exception handling tests (3 tests)

## Notes

- Wrappers are Node.js ES6 modules (`.mjs`)
- They must be executable (`chmod +x`)
- All wrappers follow a similar pattern: import module → setup environment → run → report errors
- The wrapper path in RUN lines uses `%S/../../test-wrappers/` to be relative to the test file
