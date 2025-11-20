# Lit Testing Strategies for Multi-Target Compilation

This directory contains five different strategies for handling conditional compilation and testing across multiple Cheerp targets (JS, WebAssembly, asm.js) using LLVM's lit testing framework.

## Overview

The original test approach uses explicit shell conditionals (`if [ "$HAS_JS" = "1" ]; then ... fi`) in RUN lines, which works but is verbose. These strategies demonstrate alternative approaches that reduce verbosity while maintaining or improving functionality.

## Strategy Comparison

| Strategy | Verbosity | Flexibility | Complexity | Best For |
|----------|-----------|-------------|------------|----------|
| 1. REQUIRES | Low | Low | Very Low | Simple, target-specific tests |
| 2. Python Macros | Medium | High | Medium | General purpose, clean syntax |
| 3. Script Wrapper | Very Low | Very High | Medium | Complex test logic |
| 4. Loop-based | Very Low | Very High | High | Many similar tests |
| 5. Enhanced Current | Low | High | Low | Drop-in improvement |

## Strategy 1: REQUIRES Directive

**Location:** `strategy1_requires/`

**Approach:** Uses lit's native `REQUIRES: feature` directive to conditionally run tests.

**Example:**
```cpp
// REQUIRES: wasm
// RUN: %compile_wasm -o %t/w %s
```

**Pros:**
- Simplest possible approach
- Uses lit's built-in functionality
- Very clear which tests require which features
- No shell conditionals needed

**Cons:**
- Can only gate entire test files, not individual RUN lines
- Requires splitting tests into separate files for different targets
- Less flexible than other approaches

**Usage:**
```bash
cd strategy1_requires
lit . --param TARGETS=wasm,js
```

## Strategy 2: Python-Based Macro Substitution

**Location:** `strategy2_python_macros/`

**Approach:** Uses Python regex substitutions in `lit.cfg` to create custom "macros" that expand into conditional commands.

**Example:**
```cpp
// RUN: %compile_wasm{-o %t/w %s}
// RUN: %if_wasm{test -f %t/w.wasm}
```

**Pros:**
- More flexible than REQUIRES
- Can conditionally run individual RUN lines
- Clean, readable syntax in test files
- All conditional logic centralized in lit.cfg

**Cons:**
- Slightly more complex lit.cfg setup
- Uses regex substitutions which can be tricky to debug
- Need to define macros for each target

**Usage:**
```bash
cd strategy2_python_macros
lit . --param TARGETS=wasm
```

## Strategy 3: Shell Script Wrapper

**Location:** `strategy3_script_wrapper/`

**Approach:** Uses an external shell script to handle all conditional logic and testing operations.

**Example:**
```cpp
// RUN: %test_helper --compile wasm,js --output %t --source %s
// RUN: %test_helper --execute wasm,js --output %t | %FileCheck %s
```

**Pros:**
- Cleanest test files - extremely readable
- Maximum flexibility in the helper script
- Easy to add new features without changing lit.cfg
- Can handle complex logic that's difficult in Python or shell conditionals

**Cons:**
- Requires maintaining a separate script
- Slightly more indirection when debugging
- Script needs to be distributed with tests

**Usage:**
```bash
cd strategy3_script_wrapper
lit . --param TARGETS=js,wasm
```

## Strategy 4: Loop-Based Parameterized Tests

**Location:** `strategy4_loop_based/`

**Approach:** Custom lit test format that automatically runs each test multiple times, once per enabled target.

**Example:**
```cpp
// RUN: %cheerp_compile{%target} -o %t/%target_short %s
// RUN: %execute{%target} %t/%target_short | %FileCheck %s
```

**Pros:**
- Extremely DRY - write test once, runs for all targets
- Automatically scales with new targets
- Clear separation of concerns
- Each target gets its own test result in lit output (e.g., `test.cpp::wasm`, `test.cpp::js`)

**Cons:**
- Most complex lit.cfg setup
- Requires understanding custom lit test formats
- May be overkill for simple tests

**Usage:**
```bash
cd strategy4_loop_based
lit . --param TARGETS=wasm,js
# Will show: test.cpp::wasm PASS, test.cpp::js PASS
```

## Strategy 5: Enhanced Current Approach

**Location:** `strategy5_enhanced_current/`

**Approach:** Improved version of the original approach with cleaner macro-style substitutions.

**Example:**
```cpp
// RUN: %compile_for{wasm} -o %t/w %s
// RUN: %run_if{wasm} test -f %t/w.wasm
// RUN: %run_if{js} %node %t/j 2>&1 | %FileCheck %s
```

**Pros:**
- Familiar to those using the current approach
- Clean, intuitive syntax
- Easy to understand and maintain
- Good balance of simplicity and flexibility
- Drop-in improvement over original

**Cons:**
- Still requires some regex knowledge for maintenance
- Each target needs explicit handling in lit.cfg

**Usage:**
```bash
cd strategy5_enhanced_current
lit . --param TARGETS=wasm,js
```

## Running the Tests

Each strategy directory contains:
- `cheerp_arithmetic_test.cpp` - Example test file
- `lit.cfg` - Lit configuration implementing the strategy
- Additional files (e.g., `test_helper.sh` for Strategy 3)

To run any strategy:

```bash
# Run specific strategy
cd strategy<N>_<name>
lit .

# Run with specific targets
lit . --param TARGETS=wasm

# Run with multiple targets
lit . --param TARGETS=wasm,js

# Run all strategies (from this directory)
lit .
```

## Choosing a Strategy

**For new projects:** Strategy 5 (Enhanced Current) or Strategy 2 (Python Macros) offer the best balance of simplicity and flexibility.

**For complex test logic:** Strategy 3 (Script Wrapper) provides maximum flexibility with clean test files.

**For many similar tests:** Strategy 4 (Loop-based) eliminates duplication by running the same test for all targets.

**For simple needs:** Strategy 1 (REQUIRES) is the simplest if you can split tests by target.

**For existing projects:** Strategy 5 (Enhanced Current) is the easiest migration path from the original approach.

## Original Approach Comparison

The original approach used:
```cpp
// RUN: if [ "$HAS_WASM" = "1" ]; then %cheerp_clang -target cheerp-wasm -o %t/w %s; fi
```

All strategies above reduce this verbosity significantly:

- **Strategy 1:** `// REQUIRES: wasm` + `// RUN: %compile_wasm -o %t/w %s`
- **Strategy 2:** `// RUN: %compile_wasm{-o %t/w %s}`
- **Strategy 3:** `// RUN: %test_helper --compile wasm --output %t --source %s`
- **Strategy 4:** `// RUN: %cheerp_compile{%target} -o %t/%target_short %s`
- **Strategy 5:** `// RUN: %compile_for{wasm} -o %t/w %s`

## Common Features

All strategies support:
- Dynamic target selection via `--param TARGETS=...`
- Multiple targets (js, wasm, asmjs)
- Integration with FileCheck for output verification
- Custom compiler flags via environment variables

## Environment Variables

All strategies respect these environment variables:
- `CHEERP_FLAGS` - Additional compiler flags
- `EXTRA_FLAGS` - Extra flags to append
- `FILECHECK` - Path to FileCheck binary (optional)

## Notes

- The test file used (`cheerp_arithmetic_test.cpp`) is identical in functionality across all strategies, only the RUN lines differ.
- Each strategy's `lit.cfg` contains detailed comments explaining the implementation.
- All strategies handle disabled targets gracefully by replacing commands with `true` or skipping tests.
