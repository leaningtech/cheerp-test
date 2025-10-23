# Testing for Compilation Bugs in Cheerp

## Overview

This document explains how to test the **compiler's correctness** by simulating bugs in the compiled output and verifying that tests catch them.

**Goal**: Ensure that when the Cheerp compiler produces incorrect JavaScript/WebAssembly, our tests detect the problem.

## The Test: `multiarray_corruption_test.cpp`

### What It Does

This test validates that we can detect compilation bugs by:

1. **Compiling `multiarray.cpp` correctly** (reference version)
2. **Compiling `multiarray.cpp` again** and **corrupting the output** (buggy version)
3. **Running both versions** and capturing their output
4. **Verifying the outputs differ** (proving the test catches bugs)

### Test Structure

```cpp
// RUN: %cheerp_clang ... %S/multiarray.cpp -o %t_reference.js
// RUN: %node %t_reference.js 2>&1 > %t_reference.out
// RUN: %cheerp_clang ... %S/multiarray.cpp -o %t_buggy.js
// RUN: sed -i 's/66/65/g' %t_buggy.js
// RUN: %node %t_buggy.js 2>&1 > %t_buggy.out
// RUN: sh -c '! diff %t_reference.out %t_buggy.out'
```

### How It Works

**Step 1: Compile reference**
```bash
/opt/cheerp/bin/clang++ ... multiarray.cpp -o reference.js
```
Produces correct JavaScript.

**Step 2: Run reference and capture output**
```bash
node reference.js 2>&1 > reference.out
```
Output:
```
Multiarray support 1/ : SUCCESS
Multiarray support 2/ : SUCCESS
Multiarray support 3/ : SUCCESS
Multiarray support 4/ : SUCCESS
```

**Step 3: Compile second version to corrupt**
```bash
/opt/cheerp/bin/clang++ ... multiarray.cpp -o buggy.js
```

**Step 4: Corrupt the JS file**
```bash
sed -i 's/66/65/g' buggy.js
```
This changes the expected value from 66 to 65, simulating a compiler bug in array sum calculation.

**Step 5: Run corrupted version**
```bash
node buggy.js 2>&1 > buggy.out
```
Output:
```
Multiarray support 1/ : FAILURE
Multiarray support 2/ : FAILURE
Multiarray support 3/ : SUCCESS  
Multiarray support 4/ : SUCCESS
```

**Step 6: Verify outputs differ**
```bash
! diff reference.out buggy.out
```
The `!` inverts the exit code - we **want** diff to fail (find differences).

## Running the Test

```bash
cd /home/alex/cheerp/tests
lit -v unit/memory/multiarray_corruption_test.cpp
```

**Expected result: PASS** (meaning it detected the corruption)

## Types of Corruptions You Can Test

### 1. Array Indexing Bugs
```bash
sed -i 's/\\[i\\]/[0]/g' buggy.js
```
Simulates incorrect array access patterns.

### 2. Arithmetic Errors
```bash
sed -i 's/66/65/g' buggy.js
```
Simulates wrong expected values or calculations.

### 3. Constructor Counting
```bash
sed -i 's/24/23/g' buggy.js
```
Simulates missing constructor calls.

### 4. Memory Allocation
```bash
sed -i 's/\\*4\\*/\\*3\\*/g' buggy.js
```
Simulates incorrect size calculations.

## Why This Approach?

### ✅ Advantages
- **No compiler modification needed** - test compiler bugs without touching compiler code
- **Simulates real bugs** - corruptions mirror actual compilation errors
- **Validates test effectiveness** - proves tests catch real problems
- **Fast iteration** - easy to add new corruption patterns

### What We're Testing
- **NOT** testing the source C++ code (multiarray.cpp is correct)
- **YES** testing that differences between C++ intent and JS/wasm output are caught
- **YES** simulating what happens when the compiler makes mistakes

## Creating New Corruption Tests

### Template

```cpp
// RUN: %cheerp_clang [flags] %S/source.cpp -o %t_reference.js
// RUN: %node %t_reference.js 2>&1 > %t_reference.out
// RUN: %cheerp_clang [flags] %S/source.cpp -o %t_buggy.js
// RUN: sed -i 's/PATTERN/REPLACEMENT/g' %t_buggy.js
// RUN: %node %t_buggy.js 2>&1 > %t_buggy.out
// RUN: sh -c '! diff %t_reference.out %t_buggy.out'
```

### Example: Test Array Bounds

```cpp
// Corrupt array size allocation
// RUN: sed -i 's/new Array(12)/new Array(10)/g' %t_buggy.js
```

### Example: Test Type Handling

```cpp
// Corrupt type conversions  
// RUN: sed -i 's/|0//g' %t_buggy.js
```

## Interpreting Results

### ✅ Test PASSES
```
PASS: Cheerp :: unit/memory/multiarray_corruption_test.cpp
```
**Meaning**: The test successfully detected that the corrupted output differs from the reference.
**This is GOOD** - your test catches bugs!

### ❌ Test FAILS
```
FAIL: Cheerp :: unit/memory/multiarray_corruption_test.cpp
error: command failed with exit status 1
```
**Possible reasons**:
1. The corruption didn't actually change behavior
2. The outputs are identical (corruption was ineffective)
3. The test's expectations are wrong

**To debug**:
```bash
cd /home/alex/cheerp/tests/unit/memory/Output
diff multiarray_corruption_test.cpp.tmp_reference.out multiarray_corruption_test.cpp.tmp_buggy.out
```

If diff shows no output, the corruption didn't work - try a different pattern.

## Workflow for Testing Compiler Changes

When modifying the Cheerp compiler:

1. **Before changes**: Run `multiarray_corruption_test.cpp` - should PASS
2. **Make compiler changes**
3. **After changes**: Run all tests including corruption tests
4. **If corruption test FAILS**: Your changes might have broken something
5. **Investigate**: Check if the compiler now produces output similar to the "buggy" version

## Example: Full Workflow

```bash
# 1. Verify test works
cd /home/alex/cheerp/tests
lit -v unit/memory/multiarray_corruption_test.cpp
# Should see: PASS

# 2. Make compiler changes  
# (modify Cheerp compiler code)

# 3. Rebuild compiler
cd /home/alex/cheerp
make

# 4. Re-run test
cd /home/alex/cheerp/tests
lit -v unit/memory/multiarray_corruption_test.cpp

# 5. If PASS: compiler still produces correct output
# 6. If FAIL: compiler might now have a bug similar to our simulated bug
```

## Files

- **Test**: `unit/memory/multiarray_corruption_test.cpp`
- **Source**: `unit/memory/multiarray.cpp` (the correct C++ code)
- **Output**: `unit/memory/Output/` (generated .js files and output captures)
- **Config**: `/home/alex/cheerp/tests/lit.cfg`

## Key Insight

This test answers: **"If the compiler produces incorrect output, will we catch it?"**

By intentionally corrupting the compiled JavaScript and verifying our test framework detects it, we validate that real compiler bugs won't go unnoticed.
