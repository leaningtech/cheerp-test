# Why 'true' Substitution Is Necessary in Strategy 5

## The Problem

When a user runs tests with `--param TARGETS=js` (specifying only JavaScript target), the lit framework still needs to process ALL RUN lines in the test file, including those for disabled targets (wasm, asmjs). Without proper handling, disabled macros would expand to invalid shell commands, causing test failures.

## Current Implementation (With 'true' Substitution)

### The lit.cfg Logic

```python
for target in ['wasm', 'js', 'asmjs']:
    if target in selected_targets:
        # Target ENABLED - remove the prefix
        config.substitutions.append((
            r'%run_if\{' + target + r'\} ',
            ''  # Strip the prefix - command runs
        ))
    else:
        # Target DISABLED - replace entire line with 'true'
        config.substitutions.append((
            r'%run_if\{' + target + r'\}[^\n]*',
            'true'  # No-op that succeeds
        ))
```

## Scenario Analysis: Running with `--param TARGETS=js`

### Test File Content
```cpp
// RUN: %compile_for{wasm} -O1 -o %t/w %s
// RUN: %compile_for{js} -O1 -o %t/j %s
// RUN: %run_if{wasm} test -f %t/w.wasm
// RUN: %run_if{js} test -f %t/j
```

---

## Case 1: WITH 'true' Substitution (Current - Works ✅)

### Expansion Process

When `TARGETS=js` is specified:

1. **Line 1**: `%compile_for{wasm}` → `true` (disabled target)
   ```bash
   true
   ```
   - **Result**: Command succeeds, does nothing ✅
   - **Test status**: PASS (no-op succeeds)

2. **Line 2**: `%compile_for{js}` → `/opt/cheerp/bin/clang++ -target cheerp ...` (enabled target)
   ```bash
   /opt/cheerp/bin/clang++ -target cheerp -O1 -o %t/j %s
   ```
   - **Result**: Compiles successfully ✅
   - **Test status**: PASS (compilation works)

3. **Line 3**: `%run_if{wasm} test -f %t/w.wasm` → `true` (disabled target)
   ```bash
   true
   ```
   - **Result**: Command succeeds, does nothing ✅
   - **Test status**: PASS (no-op succeeds)

4. **Line 4**: `%run_if{js} test -f %t/j` → `test -f %t/j` (enabled target)
   ```bash
   test -f /path/to/%t/j
   ```
   - **Result**: File exists, test passes ✅
   - **Test status**: PASS (file verification works)

### Overall Test Result: ✅ PASS

All RUN lines execute successfully. Disabled targets become harmless no-ops.

---

## Case 2: WITHOUT 'true' Substitution (Broken ❌)

### The Problem: Invalid Shell Commands

If we DON'T replace disabled macros with `true`, they expand to invalid commands.

### Expansion Process (Without true Replacement)

**Scenario A: If we replace with empty string instead**

```python
# Wrong approach
else:
    config.substitutions.append((
        r'%compile_for\{' + target + r'\}',
        ''  # Empty string!
    ))
```

When `TARGETS=js`:

1. **Line 1**: `%compile_for{wasm}` → `` (empty string)
   ```bash
   -O1 -o %t/w %s
   ```
   - **Result**: Shell sees `-O1` as a command to execute ❌
   - **Error**: `command not found: -O1`
   - **Test status**: FAIL (command not found)

2. **Line 3**: `%run_if{wasm} test -f %t/w.wasm` → `` (empty string)
   ```bash
   test -f %t/w.wasm
   ```
   - **Result**: Shell sees `-f` as a command ❌
   - **Error**: `command not found: -f` OR `test` fails because no arguments
   - **Test status**: FAIL

### Overall Test Result: ❌ FAIL - Multiple errors

---

**Scenario B: If we don't replace at all (leave the macro unexpanded)**

```python
# Another wrong approach - no substitution for disabled targets
# Simply skip adding a substitution
```

When `TARGETS=js`:

1. **Line 1**: `%compile_for{wasm}` → `%compile_for{wasm}` (no substitution)
   ```bash
   %compile_for{wasm} -O1 -o %t/w %s
   ```
   - **Result**: Shell sees `%compile_for{wasm}` as a command ❌
   - **Error**: `command not found: %compile_for{wasm}`
   - **Test status**: FAIL (invalid command)

2. **Line 3**: `%run_if{wasm}` → `%run_if{wasm}` (no substitution)
   ```bash
   %run_if{wasm} test -f %t/w.wasm
   ```
   - **Result**: Shell sees `%run_if{wasm}` as a command ❌
   - **Error**: `command not found: %run_if{wasm}`
   - **Test status**: FAIL (invalid command)

### Overall Test Result: ❌ FAIL - Multiple errors

---

## Why `true` is the Perfect Solution

### What is `true` in Shell?

`true` is a shell built-in command that:
- **Always succeeds** (exit code 0)
- **Does nothing** (no side effects)
- **Takes any arguments** and ignores them
- **Is universally available** in all shells (bash, sh, zsh, etc.)

### Examples of `true` behavior:

```bash
$ true
$ echo $?
0

$ true some garbage here
$ echo $?
0

$ true -O1 -o %t/w %s
$ echo $?
0
```

### Why This is Perfect for Disabled Targets

When a target is disabled:
1. ✅ The RUN line executes successfully (exit code 0)
2. ✅ No files are created or modified
3. ✅ No compilation happens
4. ✅ No runtime errors occur
5. ✅ The test continues to the next RUN line
6. ✅ Only the enabled target's output is verified

---

## Real-World Example: Debugging

### Test Run with TARGETS=js

#### With 'true' Substitution (Current Implementation)

```bash
$ cd /home/alex/cheerp/cheerp-test/experimental/lit_strategies/strategy5_enhanced_current/variant_multi_test
$ lit global_variables.cpp --param TARGETS=js -vv 2>&1 | grep "RUN:"
```

Output shows:
```
RUN: at line 3: true
+ true
RUN: at line 4: /opt/cheerp/bin/clang++ -frtti ... -target cheerp -O1 -o ... global_variables.cpp
+ /opt/cheerp/bin/clang++ -frtti ... -target cheerp -O1 -o ... global_variables.cpp
RUN: at line 5: true
+ true
RUN: at line 6: true
+ true
RUN: at line 7: %node ... 2>&1 | %FileCheck ...
+ node ... 2>&1 | /usr/bin/FileCheck-20 ...
```

**Result**: Test PASSES ✅
- Disabled wasm target lines become `true` (no-ops)
- Enabled js target lines execute normally
- Output verified with FileCheck

#### Without 'true' Substitution (Hypothetical)

If we used empty string replacement:

```bash
$ lit global_variables.cpp --param TARGETS=js -vv 2>&1 | grep -A2 "RUN:"
```

Output would show:
```
RUN: at line 3: -O1 -o ... global_variables.cpp
+ -O1: command not found

ERROR: test failed!
```

**Result**: Test FAILS ❌
- First disabled line tries to execute `-O1 -o ...` as a command
- Shell cannot find `-O1` command
- Test terminates with error

---

## The `%run_if` Macro Case

The `%run_if` macro is even more critical:

### With 'true' Substitution

```cpp
// RUN: %run_if{wasm} test -f %t/w.wasm
// RUN: %run_if{js} test -f %t/j
```

When `TARGETS=js`:

1. **Line 1**: `%run_if{wasm} test -f %t/w.wasm` → `true` (entire line replaced)
   - Just executes `true`, succeeds ✅

2. **Line 2**: `%run_if{js} test -f %t/j` → `test -f %t/j` (prefix removed)
   - Executes the file check ✅

### Without 'true' Substitution (Empty String)

```cpp
// RUN: %run_if{wasm} test -f %t/w.wasm
// RUN: %run_if{js} test -f %t/j
```

When `TARGETS=js`:

1. **Line 1**: `%run_if{wasm} test -f %t/w.wasm` → ` test -f %t/w.wasm` (just prefix removed, still invalid)
   ```bash
   test -f %t/w.wasm
   ```
   - **Problem**: No wasm file was created (target disabled), test fails ❌
   - **Error**: File not found, test returns non-zero exit code
   - **Cascading failure**: Any subsequent checks fail

### The Key Difference

**With `true`**: Disabled lines gracefully no-op
**Without `true`**: Disabled lines execute but fail because preconditions aren't met

---

## Multiple Substitution Patterns

The lit.cfg uses TWO different patterns for safety:

### Pattern 1: For `%compile_for` (exact match + rest of line)

```python
config.substitutions.append((
    r'%compile_for\{' + target + r'\}[^\n]*',  # Matches entire line
    'true'
))
```

This regex:
- Matches: `%compile_for{wasm}` 
- Plus: Everything after it until end of line (`[^\n]*`)
- Replaces: Entire matched content with `true`

**Why the `[^\n]*`?**
```cpp
// RUN: %compile_for{wasm} -O1 -o %t/w %s
                           ↑
                    Rest of line captured
```

Without `[^\n]*`, we'd only replace the macro, leaving the flags:
```bash
true -O1 -o %t/w %s  # Still broken!
```

### Pattern 2: For `%run_if` (exact match + rest of line)

```python
config.substitutions.append((
    r'%run_if\{' + target + r'\}[^\n]*',  # Matches entire line
    'true'
))
```

Same reasoning - captures the entire RUN line for disabled targets.

---

## Comparison: Different Approaches

| Approach | Behavior | Result |
|----------|----------|--------|
| **Current (true)** | Disabled: `true`, Enabled: execute | ✅ PASS |
| **Empty string** | Disabled: leaves fragments, Enabled: execute | ❌ FAIL |
| **No replacement** | Macro stays, tries to execute | ❌ FAIL |
| **Skip line** | Cannot skip lit RUN lines | ❌ FAIL |
| **Conditional RUN** | Not supported by lit framework | ❌ Impossible |

---

## Why Not Use Other Approaches?

### Approach: Use `:` (colon) instead of `true`

```bash
: # Shell no-op (colon command)
```

**Problem**: Less portable, doesn't ignore arguments well
```bash
$ : -O1 -o %t/w %s
$ echo $?
0  # Works, but less conventional
```

### Approach: Use `echo` with redirection

```bash
echo >/dev/null
```

**Problem**: More complex, unnecessary side effects
```bash
$ echo -O1 -o %t/w %s >/dev/null
-O1 -o %t/w %s  # Output visible!
```

### Approach: Use conditional execution (`if true`)

```bash
if true; then test -f %t/j; fi
```

**Problem**: More verbose, harder to read, unnecessary complexity

---

## Summary: The Necessity of 'true' Substitution

### The Core Issue
Lit processes all RUN lines regardless of which targets are enabled. Without substitution, disabled macros expand to invalid shell syntax.

### The Solution: `true`
Replace entire disabled RUN lines with the `true` command, which:
- Always succeeds (exit code 0)
- Takes any arguments and ignores them
- Is universally available
- Has no side effects
- Allows tests to continue normally

### Without `true` Substitution
- ❌ Invalid shell commands are generated
- ❌ Tests fail with "command not found" errors
- ❌ Disabled target lines cause cascading failures
- ❌ Test suite becomes unusable with target selection

### With `true` Substitution
- ✅ All RUN lines execute without errors
- ✅ Disabled targets gracefully no-op
- ✅ Enabled targets run normally
- ✅ Test results accurately reflect enabled targets
- ✅ Clean, predictable test behavior

---

## Practical Example: See It In Action

You can verify this by modifying the lit.cfg temporarily:

### Current (Working)
```python
# Disabled target - replace with 'true'
config.substitutions.append((
    r'%run_if\{' + target + r'\}[^\n]*',
    'true'
))
```

Running tests:
```bash
$ lit . --param TARGETS=js 2>&1 | tail -5
Passed: 13 (100.00%)  # ✅ All pass
```

### Broken (Without true)
```python
# Disabled target - replace with empty string
config.substitutions.append((
    r'%run_if\{' + target + r'\}[^\n]*',
    ''  # Empty!
))
```

Running tests:
```bash
$ lit . --param TARGETS=js 2>&1 | tail -10
FAIL: ... (multiple failures)
# ❌ Tests fail with command not found errors
```

---

## Conclusion

The `true` substitution is **essential** because:

1. **Lit doesn't support conditional RUN lines** - all lines are always processed
2. **Disabled targets must produce valid shell commands** - `true` is the perfect choice
3. **Alternative approaches break** - empty strings, no replacement, etc. all fail
4. **Graceful degradation** - disabled targets don't cause errors, they just no-op
5. **Clean test output** - only enabled target results are meaningful

Without this substitution, Strategy 5 would be completely broken for multi-target selection. The `true` replacement is not optional—it's the core mechanism that makes conditional target execution possible in lit.
