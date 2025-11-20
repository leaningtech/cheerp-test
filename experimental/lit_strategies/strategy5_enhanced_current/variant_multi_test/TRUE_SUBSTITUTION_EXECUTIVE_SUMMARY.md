# Executive Summary: Why 'true' Substitution is Essential

## TL;DR (Too Long; Didn't Read)

**Question**: Why replace disabled target macros with `true`?

**Answer**: Because lit processes ALL RUN lines regardless of target selection. Without `true`, disabled target lines expand to invalid shell commands, causing test failures. `true` is the only shell command that always succeeds and accepts any arguments, making it the perfect placeholder.

---

## The Core Problem

### The Situation
- User runs: `lit . --param TARGETS=js`
- Test file has RUN lines for wasm, js, and asmjs targets
- Lit expands ALL macros in ALL RUN lines, not just the enabled ones
- Disabled target macros expand to unparseable garbage

### What Happens Without 'true' Substitution

```cpp
// Test file line:
// RUN: %compile_for{wasm} -O1 -o %t/w %s

// Without 'true', disabled macros → empty string or unexpanded:
// RUN: -O1 -o %t/w %s        (empty string replacement)
// OR
// RUN: %compile_for{wasm} -O1 -o %t/w %s  (no replacement)

// Shell tries to execute:
$ -O1 -o %t/w %s
bash: -O1: command not found  ❌ FAILURE
```

---

## Why 'true' is the Solution

### What is `true`?
A shell built-in that:
- Always returns exit code 0 (success)
- Takes any number of arguments and ignores them
- Has zero side effects
- Exists in all shells (bash, sh, zsh, ksh, etc.)

### How It Works
```bash
$ true -O1 -o %t/w %s
$ echo $?
0  # ✅ Always succeeds, regardless of arguments
```

### Applied to Our Problem
```cpp
// Test file line:
// RUN: %compile_for{wasm} -O1 -o %t/w %s

// With 'true' substitution for disabled targets:
// RUN: true

// Shell executes:
$ true
$ echo $?
0  # ✅ SUCCESS - Test continues normally
```

---

## The Complete Picture: Before and After

### WITHOUT 'true' (Broken)

| Step | What Happens | Status |
|------|--------------|--------|
| 1. User: `lit . --param TARGETS=js` | Command starts | ✓ |
| 2. Lit reads config | selected_targets = {js} | ✓ |
| 3. Lit finds: `%compile_for{wasm} -O1 ...` | Macro needs substitution | ✓ |
| 4. No 'true' replacement → empty string | Becomes: `-O1 ...` | ✗ |
| 5. Shell tries to execute `-O1` | Searches PATH for `-O1` | ✗ |
| 6. Not found anywhere | Command not found error | ✗ FAIL |
| 7. Test terminates | Multiple errors | ✗ FAIL |

### WITH 'true' (Works)

| Step | What Happens | Status |
|------|--------------|--------|
| 1. User: `lit . --param TARGETS=js` | Command starts | ✓ |
| 2. Lit reads config | selected_targets = {js} | ✓ |
| 3. Lit finds: `%compile_for{wasm} -O1 ...` | Macro needs substitution | ✓ |
| 4. 'true' replacement → `true` | Disabled target becomes no-op | ✓ |
| 5. Shell executes `true` | Built-in command found | ✓ |
| 6. Returns exit code 0 | Success | ✓ |
| 7. Test continues normally | Only enabled target checked | ✓ PASS |

---

## The Three Layers of Understanding

### Layer 1: The Immediate Problem
**Lit processes all RUN lines, even for disabled targets.**

Without a substitute, the macro expands to nothing or stays unexpanded, creating invalid shell syntax.

### Layer 2: Why Not Other Solutions?

| Alternative | Why It Fails |
|-------------|-------------|
| Empty string | Leaves fragments: `-O1 -o %t/w` → command not found |
| No replacement | Macro stays: `%compile_for{wasm}` → command not found |
| `:` (colon) | Works but non-standard; less clear intent |
| `echo` | Output visible; more complex |
| `if true; then` | Verbose; overkill for a no-op |
| Skip line | Lit doesn't support conditional RUN lines |

### Layer 3: Why `true` is Perfect

```
true command properties:
├─ Exit Code: Always 0 (success) ✅
├─ Arguments: Ignores all of them ✅
├─ Side Effects: None ✅
├─ Availability: Universal ✅
├─ Clarity: Clear intent (disabled target) ✅
└─ Simplicity: Minimal overhead ✅
```

---

## How It Fails: The Cascading Error Pattern

### Scenario: TARGETS=js, but wasm macros not replaced

```
Test file:
    Line 1: // RUN: %compile_for{wasm} -O1 -o %t/w %s
    Line 2: // RUN: test -f %t/w.wasm

lit.cfg expands (WITHOUT 'true'):
    Line 1: -O1 -o %t/w %s          (empty string leaves garbage)
    Line 2: test -f %t/w.wasm       (but file never created!)

Shell execution:
    $ -O1 -o %t/w %s
    bash: -O1: command not found    ❌ FAIL at line 1
    
    [Line 2 never executes; test already failed]
```

### How It Works: With 'true' substitution

```
Test file:
    Line 1: // RUN: %compile_for{wasm} -O1 -o %t/w %s
    Line 2: // RUN: test -f %t/w.wasm

lit.cfg expands (WITH 'true'):
    Line 1: true                    (disabled target)
    Line 2: true                    (disabled target)

Shell execution:
    $ true
    (exit code 0)                   ✅ Continue
    
    $ true
    (exit code 0)                   ✅ Continue
    
    [Both lines execute without error; test passes]
```

---

## Real-World Consequences

### Without 'true' Substitution
```bash
$ lit . --param TARGETS=js
FAIL: 13 failed in 2.3s
Error: command not found: -O1
Error: command not found: %run_if
Error: command not found: -target
```

**Result**: Complete test suite failure. Multi-target selection is broken.

### With 'true' Substitution
```bash
$ lit . --param TARGETS=js
PASS: 13 passed in 7.2s
```

**Result**: All tests pass. Multi-target selection works perfectly.

---

## The Regex Pattern: Why `[^\n]*` Matters

### Without `[^\n]*` (Broken)
```python
config.substitutions.append((
    r'%compile_for\{' + target + r'\}',  # Only matches the macro
    'true'
))

# Result: %compile_for{wasm} -O1 -o %t/w %s
#         ↓ (only macro replaced)
#         true -O1 -o %t/w %s
#              ↑ Still garbage!
```

### With `[^\n]*` (Correct)
```python
config.substitutions.append((
    r'%compile_for\{' + target + r'\}[^\n]*',  # Matches macro + rest of line
    'true'
))

# Result: %compile_for{wasm} -O1 -o %t/w %s
#         ↓ (entire line replaced)
#         true
#         ✅ Clean!
```

---

## Decision Matrix: When to Use 'true' Substitution

### Use 'true' substitution when:
- ✅ You want to support target selection (--param TARGETS=...)
- ✅ You have multiple targets in the same test file
- ✅ You want tests to pass when some targets are disabled
- ✅ You want clean, predictable test behavior
- ✅ You want minimal overhead for disabled targets

### Don't use it when:
- ✗ All tests are single-target only
- ✗ You're using only one compilation target
- ✗ You don't support target selection

---

## Summary: The Essential Three Points

### 1. The Problem
**Lit processes ALL RUN lines regardless of target selection. Disabled macros must expand to something valid.**

### 2. The Constraint
**The replacement must:**
- Always succeed (exit code 0)
- Accept any arguments
- Have no side effects
- Work universally

### 3. The Solution
**`true` is the ONLY standard shell command that satisfies all constraints.**

---

## Conclusion

The `true` substitution is **not optional—it's fundamental** to making Strategy 5 work with multi-target selection.

Without it:
- ❌ Tests fail with "command not found" errors
- ❌ Target selection is unusable
- ❌ Multi-target testing breaks completely

With it:
- ✅ Disabled targets gracefully no-op
- ✅ Enabled targets execute normally
- ✅ Test suite works reliably
- ✅ Clean, predictable behavior

The `true` command is the elegant solution that allows the same test file to run across multiple targets without modification.
