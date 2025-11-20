# Quick Reference: Strategy Comparison

## Side-by-Side Comparison

### Original Approach (Verbose)
```cpp
// RUN: mkdir -p %t
// RUN: if [ "$HAS_WASM" = "1" ]; then %cheerp_clang -target cheerp-wasm -o %t/w %s; fi
// RUN: if [ "$HAS_JS" = "1" ]; then %cheerp_clang -target cheerp -o %t/j %s; fi
// RUN: if [ "$HAS_WASM" = "1" ]; then test -f %t/w.wasm && test -f %t/w; fi
// RUN: if [ "$HAS_WASM" = "1" ]; then xxd -l 8 %t/w.wasm | %FileCheck %s --check-prefix=WASM-MAGIC; fi
// RUN: if [ "$HAS_JS" = "1" ]; then test -f %t/j; fi
// RUN: if [ "$HAS_JS" = "1" ]; then %node %t/j 2>&1 | %FileCheck %s --check-prefix=EXEC; fi
```

---

### Strategy 1: REQUIRES Directive
```cpp
// RUN: mkdir -p %t
// RUN: %cheerp_clang -target cheerp-wasm -o %t/w %s
// RUN: %cheerp_clang -target cheerp -o %t/j %s

// REQUIRES: wasm
// RUN: test -f %t/w.wasm && test -f %t/w
// RUN: xxd -l 8 %t/w.wasm | %FileCheck %s --check-prefix=WASM-MAGIC

// REQUIRES: js
// RUN: test -f %t/j
// RUN: %node %t/j 2>&1 | %FileCheck %s --check-prefix=EXEC
```

**Key Feature:** Gates sections of the test file with `REQUIRES:` directives

---

### Strategy 2: Python Macros
```cpp
// RUN: mkdir -p %t
// RUN: %compile_wasm{-o %t/w %s}
// RUN: %compile_js{-o %t/j %s}
// RUN: %if_wasm{test -f %t/w.wasm && test -f %t/w}
// RUN: %if_wasm{xxd -l 8 %t/w.wasm | %FileCheck %s --check-prefix=WASM-MAGIC}
// RUN: %if_js{test -f %t/j}
// RUN: %if_js{%node %t/j 2>&1 | %FileCheck %s --check-prefix=EXEC}
```

**Key Feature:** Macros with braces `%macro{command}` expand to full commands or `true`

---

### Strategy 3: Script Wrapper
```cpp
// RUN: mkdir -p %t
// RUN: %test_helper --compile wasm,js --output %t --source %s
// RUN: %test_helper --verify wasm --output %t --check-magic
// RUN: %test_helper --verify js --output %t --check-output
// RUN: %test_helper --execute wasm,js --output %t | %FileCheck %s --check-prefix=EXEC
```

**Key Feature:** Single helper script handles all logic; cleanest test files

---

### Strategy 4: Loop-Based Parameterized
```cpp
// RUN: mkdir -p %t
// RUN: %cheerp_compile{%target} -o %t/%target_short %s
// RUN: %verify_output{%target} %t/%target_short
// RUN: %execute{%target} %t/%target_short 2>&1 | %FileCheck %s --check-prefix=EXEC
```

**Key Feature:** Test runs once per target with `%target` substitution; reports separate results per target

---

### Strategy 5: Enhanced Current
```cpp
// RUN: mkdir -p %t
// RUN: %compile_for{wasm} -o %t/w %s
// RUN: %compile_for{js} -o %t/j %s
// RUN: %run_if{wasm} test -f %t/w.wasm && test -f %t/w
// RUN: %run_if{wasm} xxd -l 8 %t/w.wasm | %FileCheck %s --check-prefix=WASM-MAGIC
// RUN: %run_if{js} test -f %t/j
// RUN: %run_if{js} %node %t/j 2>&1 | %FileCheck %s --check-prefix=EXEC
```

**Key Feature:** `%run_if{target}` prefix removes conditional wrapper; drop-in replacement

---

## Line Count Comparison

For a typical multi-target test:

| Strategy | RUN Lines | Complexity | Readability |
|----------|-----------|------------|-------------|
| Original | 7-10 | Medium | Low |
| Strategy 1 | 5-7 | Very Low | High |
| Strategy 2 | 5-7 | Low | High |
| Strategy 3 | 3-5 | Low | Very High |
| Strategy 4 | 2-4 | Medium | High |
| Strategy 5 | 5-7 | Low | High |

## When to Use Each Strategy

### Strategy 1 (REQUIRES)
✅ Use when:
- Tests are naturally separated by target
- You want maximum simplicity
- Tests don't share much common code

❌ Avoid when:
- Same test logic applies to all targets
- Tests have complex interdependencies

### Strategy 2 (Python Macros)
✅ Use when:
- You want clean, self-documenting test syntax
- Individual RUN lines need conditional execution
- Comfortable with regex in Python

❌ Avoid when:
- Team unfamiliar with Python regex
- Need extremely complex conditional logic

### Strategy 3 (Script Wrapper)
✅ Use when:
- Tests involve complex multi-step operations
- Want to abstract away implementation details
- Need maximum flexibility for future changes

❌ Avoid when:
- Want minimal external dependencies
- Team prefers inline test logic

### Strategy 4 (Loop-Based)
✅ Use when:
- Many tests with identical structure
- Want separate pass/fail per target
- Need to scale to many targets

❌ Avoid when:
- Tests vary significantly between targets
- Team unfamiliar with custom lit formats

### Strategy 5 (Enhanced Current)
✅ Use when:
- Migrating from original approach
- Want familiar syntax with less verbosity
- Need quick win with minimal changes

❌ Avoid when:
- Starting from scratch (consider Strategy 2 or 3)

## Migration Difficulty

From Original Approach:

1. **Strategy 5**: ⭐ Easiest - mostly search/replace
2. **Strategy 2**: ⭐⭐ Easy - update syntax patterns
3. **Strategy 1**: ⭐⭐⭐ Medium - may need to split tests
4. **Strategy 3**: ⭐⭐⭐⭐ Medium-Hard - write wrapper script
5. **Strategy 4**: ⭐⭐⭐⭐⭐ Hardest - significant refactoring

## Recommended Starting Points

**For most projects:** Start with **Strategy 5** (Enhanced Current)
- Easy migration path
- Immediate verbosity reduction
- Can switch to other strategies later

**For new projects:** Choose **Strategy 2** (Python Macros) or **Strategy 3** (Script Wrapper)
- Strategy 2: Best balance of simplicity and power
- Strategy 3: Best long-term maintainability

**For test suites with 50+ similar tests:** Consider **Strategy 4** (Loop-Based)
- Pays off with scale
- Cleaner test results

## Testing the Strategies

Try each strategy with different target combinations:

```bash
# Test with all targets
lit strategy5_enhanced_current/

# Test with only WASM
lit strategy5_enhanced_current/ --param TARGETS=wasm

# Test with JS and WASM
lit strategy5_enhanced_current/ --param TARGETS=js,wasm

# Compare output verbosity
lit strategy1_requires/ -v
lit strategy2_python_macros/ -v
lit strategy3_script_wrapper/ -v
```

## Real-World Example: Converting a Test

**Original (10 lines):**
```cpp
// RUN: if [ "$HAS_WASM" = "1" ]; then %cheerp_clang -target cheerp-wasm -o %t/w %s; fi
// RUN: if [ "$HAS_JS" = "1" ]; then %cheerp_clang -target cheerp -o %t/j %s; fi
// RUN: if [ "$HAS_WASM" = "1" ]; then test -f %t/w.wasm; fi
// ... 7 more similar lines
```

**Strategy 5 (6 lines):**
```cpp
// RUN: %compile_for{wasm} -o %t/w %s
// RUN: %compile_for{js} -o %t/j %s
// RUN: %run_if{wasm} test -f %t/w.wasm
// ... 3 more lines
```

**Strategy 3 (3 lines):**
```cpp
// RUN: %test_helper --compile wasm,js --output %t --source %s
// RUN: %test_helper --verify wasm,js --output %t
// RUN: %test_helper --execute wasm,js --output %t | %FileCheck %s
```

**Savings:** 40-70% reduction in line count with improved readability
