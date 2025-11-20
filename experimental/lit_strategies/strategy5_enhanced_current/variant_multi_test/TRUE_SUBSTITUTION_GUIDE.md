# True Substitution - Complete Documentation Guide

This directory contains comprehensive documentation explaining why the `true` substitution is necessary in Strategy 5 for multi-target test execution.

## Quick Navigation

### For the Impatient (2-3 minutes)
→ **[TRUE_SUBSTITUTION_EXECUTIVE_SUMMARY.md](TRUE_SUBSTITUTION_EXECUTIVE_SUMMARY.md)**
- TL;DR explanation
- Core problem and solution
- Real-world consequences
- When to use it

### For Visual Learners (5-10 minutes)
→ **[TRUE_SUBSTITUTION_DIAGRAMS.md](TRUE_SUBSTITUTION_DIAGRAMS.md)**
- Flowcharts showing macro expansion
- State diagrams
- Timeline diagrams
- Before/after scenarios
- Visual comparison of working vs. broken

### For Deep Understanding (15-20 minutes)
→ **[WHY_TRUE_SUBSTITUTION.md](WHY_TRUE_SUBSTITUTION.md)**
- Complete detailed explanation
- Multiple scenarios (working, empty string, no replacement)
- Regex pattern explanation
- Why other approaches fail
- Practical debugging examples

---

## The Core Concept in One Paragraph

When users run `lit . --param TARGETS=js` to test only JavaScript targets, lit still processes all RUN lines in the test file—including those for disabled targets (wasm, asmjs). Without proper substitution, disabled macros expand to invalid shell commands (like `-O1 -o %t/w %s` or `%compile_for{wasm}`), causing tests to fail with "command not found" errors. The solution is to replace disabled target macros with the `true` command, which always succeeds and accepts any arguments, allowing tests to continue normally and only the enabled target results to be verified.

---

## Document Structure

### WHY_TRUE_SUBSTITUTION.md (12 KB)
**Length**: Long-form detailed explanation  
**Best for**: Complete understanding and references  
**Contains**:
- Detailed problem description with examples
- Case 1: With 'true' substitution (works) ✅
- Case 2: Without 'true' substitution (broken) ❌
- Why `true` is perfect (properties table)
- Real-world debugging example
- The `%run_if` macro edge cases
- Regex pattern explanation and importance
- Comparison of different approaches
- Summary of benefits

**Key Sections**:
- Lines 1-76: Problem and current implementation
- Lines 79-116: Case 1 with 'true' (working scenario)
- Lines 79-145: Case 2 without 'true' (broken scenarios A and B)
- Lines 149-173: Why 'true' is perfect
- Lines 187-237: Real-world debugging examples
- Lines 283-323: Regex patterns and why `[^\n]*` is critical

### TRUE_SUBSTITUTION_DIAGRAMS.md (20 KB)
**Length**: Visual flowcharts and diagrams  
**Best for**: Visual understanding of the flow  
**Contains**:
- Overview pipeline diagram
- Scenario flowchart (with 'true' - works) ✅
- Scenario flowchart (without 'true' - broken) ❌
- State diagram (decision tree)
- Regex matching visualizations
- Command execution step-by-step
- Timeline diagram with events
- Summary visual

**Key Diagrams**:
- Lines 3-26: Full pipeline overview
- Lines 30-91: Complete working scenario flowchart
- Lines 95-189: Complete broken scenario flowchart (two approaches)
- Lines 193-237: Decision tree state diagram
- Lines 241-290: Regex matching explanation
- Lines 294-342: Command execution step-by-step
- Lines 346-390: Timeline comparison
- Lines 394-407: Summary visual

### TRUE_SUBSTITUTION_EXECUTIVE_SUMMARY.md (7.4 KB)
**Length**: Concise executive summary  
**Best for**: Quick reference and decision making  
**Contains**:
- TL;DR at top
- Core problem
- Why 'true' is the solution
- Before/after comparison table
- Three layers of understanding
- How it fails without 'true'
- Real-world consequences
- Regex pattern comparison
- Decision matrix (when to use)
- Essential three points summary

**Key Sections**:
- Lines 1-8: TL;DR
- Lines 11-33: Core problem
- Lines 37-65: Why 'true' is solution
- Lines 69-93: Complete before/after table
- Lines 97-125: Three layers of understanding
- Lines 129-168: Failure and success scenarios
- Lines 172-191: Real-world consequences
- Lines 225-237: Decision matrix

---

## Reading Recommendations by Use Case

### "I just need to understand what's happening" (5 min)
1. Read: **EXECUTIVE_SUMMARY.md** lines 1-8 (TL;DR)
2. Read: **EXECUTIVE_SUMMARY.md** lines 69-93 (Before/After table)
3. Done! ✅

### "I want to understand the full concept" (15 min)
1. Skim: **EXECUTIVE_SUMMARY.md** (entire, 7.4 KB)
2. Look at: **DIAGRAMS.md** lines 30-91 (flowchart)
3. Read: **WHY_TRUE_SUBSTITUTION.md** lines 39-76 (core explanation)
4. Done! ✅

### "I need to implement something similar" (30 min)
1. Read: **WHY_TRUE_SUBSTITUTION.md** (entire, 12 KB)
2. Study: **DIAGRAMS.md** (entire, 20 KB)
3. Reference: **EXECUTIVE_SUMMARY.md** (key points)
4. Done! ✅

### "I need to debug a problem" (varies)
1. See: **EXECUTIVE_SUMMARY.md** "Real-World Consequences" section
2. Check: **WHY_TRUE_SUBSTITUTION.md** "Real-World Example: Debugging"
3. Review: **DIAGRAMS.md** timeline diagram
4. Done! ✅

### "I need to explain this to someone else" (prep time)
1. Use: **EXECUTIVE_SUMMARY.md** (concepts)
2. Show: **DIAGRAMS.md** (visualizations)
3. Reference: **WHY_TRUE_SUBSTITUTION.md** (details)
4. Done! ✅

---

## Key Insights

### The Problem
- Lit processes **all** RUN lines, not just those for selected targets
- Disabled macros must expand to **something valid**
- Without 'true', they expand to **garbage or stay unexpanded**

### The Solution
- `true` is a shell built-in that **always succeeds**
- It **accepts any arguments and ignores them**
- It has **no side effects**
- It **exists everywhere** (universal availability)

### Why It Works
```
Disabled Target + 'true' Substitution = Harmless No-op ✅
Disabled Target + No Substitution = Shell Error ❌
Disabled Target + Empty String = Shell Error ❌
```

### Critical Detail
The regex pattern must include `[^\n]*` to capture the **entire line**, not just the macro:
```python
# Wrong: only replaces the macro
r'%compile_for\{wasm\}'
# Result: true -O1 -o %t/w %s  (still broken!)

# Correct: replaces entire line to newline
r'%compile_for\{wasm\}[^\n]*'
# Result: true  (clean!)
```

---

## Practical Examples

### Disabled Target Without 'true'
```bash
# In lit.cfg (WRONG):
config.substitutions.append((
    r'%compile_for\{wasm\}',
    ''  # Empty string
))

# Test file:
// RUN: %compile_for{wasm} -O1 -o %t/w %s

# After substitution (broken):
// RUN: -O1 -o %t/w %s

# Shell execution:
$ -O1 -o %t/w %s
bash: -O1: command not found  ❌
```

### Disabled Target With 'true'
```bash
# In lit.cfg (CORRECT):
config.substitutions.append((
    r'%compile_for\{wasm\}[^\n]*',
    'true'
))

# Test file:
// RUN: %compile_for{wasm} -O1 -o %t/w %s

# After substitution (working):
// RUN: true

# Shell execution:
$ true
(exit code 0)  ✅
```

---

## Common Questions

### Q: Can't we just skip the line?
A: No. Lit doesn't support conditional RUN lines. It processes all lines in all test files.

### Q: Why not use `:` (colon) instead of `true`?
A: `true` is more standard, clearer in intent, and widely recognized. `:` works but is less conventional.

### Q: What if we use `echo >/dev/null`?
A: Too complex, produces output even redirected, unnecessary overhead. `true` is simpler.

### Q: Why does the regex need `[^\n]*`?
A: To capture and replace the entire line content. Without it, compiler flags remain and cause errors.

### Q: What if we replace with nothing (empty)?
A: It leaves fragments of the line (like `-O1 -o %t/w %s`) that shell tries to execute, causing "command not found" errors.

### Q: Is `true` universally available?
A: Yes. It's a POSIX shell built-in available in bash, sh, zsh, ksh, and all standards-compliant shells.

---

## For Developers

### If you're implementing Strategy 5 elsewhere:
1. Always use `true` for disabled target substitution
2. Always include `[^\n]*` in the regex to capture entire lines
3. Test with `--param TARGETS=singleTarget` to verify
4. Check verbose output with `-vv` to see expanded commands

### If you're debugging Strategy 5:
1. Run: `lit test.cpp -vv` to see expanded commands
2. Look for: Lines that became `true` (disabled) vs. actual commands (enabled)
3. Verify: Disabled target lines show `true` in output
4. Check: Test passes with mixed target selection

### If you're testing the implementation:
1. Test single target: `lit . --param TARGETS=js` (expect PASS)
2. Test multiple targets: `lit . --param TARGETS=wasm,js` (expect PASS)
3. Test all targets: `lit .` (expect PASS)
4. Verify verbose: `lit test.cpp -vv` shows correct substitution

---

## Documentation Maintenance

### When to Update These Docs:
- ✅ If lit behavior changes regarding RUN lines
- ✅ If a better no-op command is discovered
- ✅ If the Strategy 5 implementation changes
- ✅ If new edge cases are discovered

### When NOT to Update:
- ❌ These are stable; the problem and solution are fundamental
- ❌ Only update if the actual substitution mechanism changes

---

## Version History

| Date | Change | Author |
|------|--------|--------|
| 2025-11-06 | Initial documentation | AI Assistant |
| | Comprehensive explanation | |
| | Diagrams and flowcharts | |
| | Executive summary | |
| | This guide | |

---

## Related Documentation

Located in this same directory:
- `README.md` - User guide for running tests
- `IMPLEMENTATION_SUMMARY.md` - Details of Strategy 5 implementation
- `MIGRATION_COMPLETE.md` - Summary of test migration
- `lit.cfg` - The actual implementation with comments

---

## Summary

The `true` substitution is **essential** for Strategy 5's multi-target support. It solves the fundamental problem of lit processing all RUN lines by ensuring disabled target lines expand to valid, harmless commands that always succeed. Without it, multi-target selection would be impossible. With it, tests work cleanly and predictably.

**The bottom line**: Don't skip the `true` substitution. It's not optional—it's the core mechanism making conditional target execution possible.
