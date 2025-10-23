# Corruption Test Framework - Usage Guide

## Purpose

These tests verify that when the Cheerp compiler produces incorrect output, the test framework catches it. They work by:
1. Compiling a test file correctly (reference)
2. Compiling again and corrupting the output (simulated bug)
3. Verifying the outputs differ

## Current Working Example

**File:** `multiarray_corruption_test.cpp`
- **Target:** `multiarray.cpp`
- **Corruption:** Changes `66` to `65` (array sum expected value)
- **Result:** Test PASSES (detects difference)

## How to Create Corruption Tests for Other Files

### Step 1: Copy the Template

```bash
cp multiarray_corruption_test.cpp <testname>_corruption_test.cpp
```

### Step 2: Identify the Target Test File

Look at the test file and find values that are:
- Expected results in assertions
- Array sizes or indices
- Counter values
- Numeric constants used in checks

### Step 3: Update the Template

Edit three lines in your new corruption test:

**Line 14:** Change the target file
```cpp
// FROM:
// RUN: ... %S/multiarray.cpp -o %t_reference.js

// TO:
// RUN: ... %S/test1.cpp -o %t_reference.js
```

**Line 16:** Change the target file (second compilation)
```cpp
// FROM:
// RUN: ... %S/multiarray.cpp -o %t_buggy.js

// TO:
// RUN: ... %S/test1.cpp -o %t_buggy.js
```

**Line 17:** Choose an appropriate corruption pattern
```cpp
// FROM:
// RUN: sed -i 's/66/65/g' %t_buggy.js

// TO (examples):
// RUN: sed -i 's/42/41/g' %t_buggy.js
// RUN: sed -i 's/100/99/g' %t_buggy.js
// RUN: sed -i 's/24/23/g' %t_buggy.js
```

**Lines 21-22:** Update documentation
```cpp
// TARGET: test1.cpp
// CORRUPTION: Changes expected value 42 to 41
```

### Step 4: Test Your Corruption

```bash
lit -v unit/memory/<testname>_corruption_test.cpp
```

**Expected: PASS** means it detected the corruption!

**If FAIL:** Your corruption didn't change behavior or caused a crash. Try a different value.

## Choosing Good Corruption Patterns

### ✅ Good Corruptions

These change test behavior without crashing:

```bash
# Change expected values in assertions
's/66/65/g'        # array sum
's/42/41/g'        # magic number
's/100/99/g'       # percentage/count

# Change array/counter bounds
's/24/23/g'        # constructor count
's/12/11/g'        # array size
```

### ❌ Bad Corruptions

These often crash or don't change behavior:

```bash
# Strings (often optimized away)
's/"SUCCESS"/"FAILURE"/g'

# Small numbers (too common, breaks structure)
's/1/0/g'
's/2/1/g'

# Array indices that cause bounds errors
's/\\b6\\b/5/g'   # Can cause OutOfBounds crashes
```

## Debugging Failed Corruptions

### If the test FAILS (should PASS):

```bash
cd /home/alex/cheerp/tests/unit/memory/Output

# Check if outputs actually differ
diff <testname>_corruption_test.cpp.tmp_reference.out \
     <testname>_corruption_test.cpp.tmp_buggy.out
```

**No output?** The corruption didn't work. Try:
1. Different numeric value
2. Check what the test actually checks for
3. Look at the .out files to see what's printed

### If the buggy version crashes:

Your corruption broke the program structure. Try:
- A less aggressive pattern
- Target assertion values, not execution logic
- Use larger, less common numbers

## Example Workflow

### Creating a Corruption Test for test3.cpp

1. **Read test3.cpp** to understand what it tests
2. **Find numeric constants** (let's say it checks for value `77`)
3. **Copy template:**
   ```bash
   cp multiarray_corruption_test.cpp test3_corruption_test.cpp
   ```
4. **Edit lines 14, 16, 17:**
   ```cpp
   // Line 14 & 16: Change multiarray.cpp -> test3.cpp
   // Line 17: Change 's/66/65/g' -> 's/77/76/g'
   ```
5. **Test:**
   ```bash
   lit -v unit/memory/test3_corruption_test.cpp
   ```
6. **If PASS:** Success! The test catches compilation bugs.
7. **If FAIL:** Try corrupting a different value.

## Common Test Types and Patterns

| Test Type | Good Corruption Examples |
|-----------|-------------------------|
| Array operations | `'s/66/65/g'` (sum), `'s/12/11/g'` (count) |
| Constructor counting | `'s/24/23/g'`, `'s/12/11/g'` |
| Memory allocation | `'s/100/99/g'` (size) |
| Pointer arithmetic | `'s/42/41/g'` (offset) |

## Key Insight

The goal is to corrupt values that **the test explicitly checks**, not the program logic. This way:
- ✅ The corrupted version still runs
- ✅ But produces different output  
- ✅ Proving the test would catch real compiler bugs

## Files in This Directory

- `multiarray_corruption_test.cpp` - Working reference implementation
- `test1_corruption_test.cpp` - Example (may need pattern adjustment)
- `COMPILATION_BUG_TESTING.md` - Detailed technical documentation
- This README - Quick start guide
