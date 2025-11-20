# Syntax Comparison Across All Strategies

This document provides a visual side-by-side comparison of how common testing operations are expressed in each strategy.

## Conditional Compilation

### Compile for WASM target

| Strategy | Syntax |
|----------|--------|
| **Original** | `// RUN: if [ "$HAS_WASM" = "1" ]; then %cheerp_clang -target cheerp-wasm -o %t/w %s; fi` |
| **Strategy 1** | `// RUN: %cheerp_clang -target cheerp-wasm -o %t/w %s` (with `REQUIRES: wasm`) |
| **Strategy 2** | `// RUN: %compile_wasm{-o %t/w %s}` |
| **Strategy 3** | `// RUN: %test_helper --compile wasm --output %t --source %s` |
| **Strategy 4** | `// RUN: %cheerp_compile{%target} -o %t/%target_short %s` |
| **Strategy 5** | `// RUN: %compile_for{wasm} -o %t/w %s` |

### Compile for JS target

| Strategy | Syntax |
|----------|--------|
| **Original** | `// RUN: if [ "$HAS_JS" = "1" ]; then %cheerp_clang -target cheerp -o %t/j %s; fi` |
| **Strategy 1** | `// RUN: %cheerp_clang -target cheerp -o %t/j %s` (with `REQUIRES: js`) |
| **Strategy 2** | `// RUN: %compile_js{-o %t/j %s}` |
| **Strategy 3** | `// RUN: %test_helper --compile js --output %t --source %s` |
| **Strategy 4** | `// RUN: %cheerp_compile{%target} -o %t/%target_short %s` |
| **Strategy 5** | `// RUN: %compile_for{js} -o %t/j %s` |

## Conditional Execution

### Run test only if WASM is enabled

| Strategy | Syntax |
|----------|--------|
| **Original** | `// RUN: if [ "$HAS_WASM" = "1" ]; then test -f %t/w.wasm; fi` |
| **Strategy 1** | `// REQUIRES: wasm`<br>`// RUN: test -f %t/w.wasm` |
| **Strategy 2** | `// RUN: %if_wasm{test -f %t/w.wasm}` |
| **Strategy 3** | `// RUN: %test_helper --verify wasm --output %t` |
| **Strategy 4** | `// RUN: %verify_output{%target} %t/%target_short` |
| **Strategy 5** | `// RUN: %run_if{wasm} test -f %t/w.wasm` |

### Execute and check output

| Strategy | Syntax |
|----------|--------|
| **Original** | `// RUN: if [ "$HAS_WASM" = "1" ]; then %node %t/w \| %FileCheck %s; fi` |
| **Strategy 1** | `// REQUIRES: wasm`<br>`// RUN: %node %t/w \| %FileCheck %s` |
| **Strategy 2** | `// RUN: %if_wasm{%node %t/w \| %FileCheck %s}` |
| **Strategy 3** | `// RUN: %test_helper --execute wasm --output %t \| %FileCheck %s` |
| **Strategy 4** | `// RUN: %execute{%target} %t/%target_short \| %FileCheck %s` |
| **Strategy 5** | `// RUN: %run_if{wasm} %node %t/w \| %FileCheck %s` |

## Multi-Target Operations

### Compile for multiple targets

| Strategy | Syntax |
|----------|--------|
| **Original** | `// RUN: if [ "$HAS_WASM" = "1" ]; then %cheerp_clang -target cheerp-wasm -o %t/w %s; fi`<br>`// RUN: if [ "$HAS_JS" = "1" ]; then %cheerp_clang -target cheerp -o %t/j %s; fi` |
| **Strategy 1** | `// RUN: %cheerp_clang -target cheerp-wasm -o %t/w %s`<br>`// RUN: %cheerp_clang -target cheerp -o %t/j %s` |
| **Strategy 2** | `// RUN: %compile_wasm{-o %t/w %s}`<br>`// RUN: %compile_js{-o %t/j %s}` |
| **Strategy 3** | `// RUN: %test_helper --compile wasm,js --output %t --source %s` |
| **Strategy 4** | `// RUN: %cheerp_compile{%target} -o %t/%target_short %s`<br>(runs once per target automatically) |
| **Strategy 5** | `// RUN: %compile_for{wasm} -o %t/w %s`<br>`// RUN: %compile_for{js} -o %t/j %s` |

### Execute all enabled targets

| Strategy | Syntax |
|----------|--------|
| **Original** | `// RUN: if [ "$HAS_WASM" = "1" ]; then %node %t/w; fi`<br>`// RUN: if [ "$HAS_JS" = "1" ]; then %node %t/j; fi` |
| **Strategy 1** | `// REQUIRES: wasm`<br>`// RUN: %node %t/w`<br>...<br>`// REQUIRES: js`<br>`// RUN: %node %t/j` |
| **Strategy 2** | `// RUN: %if_wasm{%node %t/w}`<br>`// RUN: %if_js{%node %t/j}` |
| **Strategy 3** | `// RUN: %test_helper --execute wasm,js --output %t` |
| **Strategy 4** | `// RUN: %execute{%target} %t/%target_short`<br>(runs once per target automatically) |
| **Strategy 5** | `// RUN: %run_if{wasm} %node %t/w`<br>`// RUN: %run_if{js} %node %t/j` |

## Complete Test Example

Here's a complete test comparing all strategies:

### Original (10 lines)
```cpp
// RUN: mkdir -p %t
// RUN: if [ "$HAS_WASM" = "1" ]; then %cheerp_clang -target cheerp-wasm -o %t/w %s; fi
// RUN: if [ "$HAS_JS" = "1" ]; then %cheerp_clang -target cheerp -o %t/j %s; fi
// RUN: if [ "$HAS_WASM" = "1" ]; then test -f %t/w.wasm && test -f %t/w; fi
// RUN: if [ "$HAS_WASM" = "1" ]; then xxd -l 8 %t/w.wasm | %FileCheck %s --check-prefix=WASM-MAGIC; fi
// RUN: if [ "$HAS_WASM" = "1" ]; then cat %t/w | %FileCheck %s --check-prefix=WASM-LOADER; fi
// RUN: if [ "$HAS_WASM" = "1" ]; then %node %t/w 2>&1 | %FileCheck %s --check-prefix=EXEC; fi
// RUN: if [ "$HAS_JS" = "1" ]; then test -f %t/j; fi
// RUN: if [ "$HAS_JS" = "1" ]; then cat %t/j | %FileCheck %s --check-prefix=JS-OUTPUT; fi
// RUN: if [ "$HAS_JS" = "1" ]; then %node %t/j 2>&1 | %FileCheck %s --check-prefix=EXEC; fi
```

### Strategy 1 (11 lines, but more readable)
```cpp
// RUN: mkdir -p %t
// RUN: %cheerp_clang -target cheerp-wasm -o %t/w %s
// RUN: %cheerp_clang -target cheerp -o %t/j %s

// REQUIRES: wasm
// RUN: test -f %t/w.wasm && test -f %t/w
// RUN: xxd -l 8 %t/w.wasm | %FileCheck %s --check-prefix=WASM-MAGIC
// RUN: cat %t/w | %FileCheck %s --check-prefix=WASM-LOADER
// RUN: %node %t/w 2>&1 | %FileCheck %s --check-prefix=EXEC

// REQUIRES: js
// RUN: test -f %t/j
// RUN: cat %t/j | %FileCheck %s --check-prefix=JS-OUTPUT
// RUN: %node %t/j 2>&1 | %FileCheck %s --check-prefix=EXEC
```

### Strategy 2 (10 lines)
```cpp
// RUN: mkdir -p %t
// RUN: %compile_wasm{-o %t/w %s}
// RUN: %compile_js{-o %t/j %s}
// RUN: %if_wasm{test -f %t/w.wasm && test -f %t/w}
// RUN: %if_wasm{xxd -l 8 %t/w.wasm | %FileCheck %s --check-prefix=WASM-MAGIC}
// RUN: %if_wasm{cat %t/w | %FileCheck %s --check-prefix=WASM-LOADER}
// RUN: %if_wasm{%node %t/w 2>&1 | %FileCheck %s --check-prefix=EXEC}
// RUN: %if_js{test -f %t/j}
// RUN: %if_js{cat %t/j | %FileCheck %s --check-prefix=JS-OUTPUT}
// RUN: %if_js{%node %t/j 2>&1 | %FileCheck %s --check-prefix=EXEC}
```

### Strategy 3 (6 lines) ⭐ Most Concise
```cpp
// RUN: mkdir -p %t
// RUN: %test_helper --compile wasm,js --output %t --source %s
// RUN: %test_helper --verify wasm --output %t --check-magic
// RUN: %test_helper --verify wasm --output %t --check-loader
// RUN: %test_helper --verify js --output %t --check-output
// RUN: %test_helper --execute wasm,js --output %t | %FileCheck %s --check-prefix=EXEC
```

### Strategy 4 (4 lines) ⭐ Most DRY
```cpp
// RUN: mkdir -p %t
// RUN: %cheerp_compile{%target} -o %t/%target_short %s
// RUN: %verify_output{%target} %t/%target_short
// RUN: %execute{%target} %t/%target_short 2>&1 | %FileCheck %s --check-prefix=EXEC
```

### Strategy 5 (10 lines) ⭐ Best Balance
```cpp
// RUN: mkdir -p %t
// RUN: %compile_for{wasm} -o %t/w %s
// RUN: %compile_for{js} -o %t/j %s
// RUN: %run_if{wasm} test -f %t/w.wasm && test -f %t/w
// RUN: %run_if{wasm} xxd -l 8 %t/w.wasm | %FileCheck %s --check-prefix=WASM-MAGIC
// RUN: %run_if{wasm} cat %t/w | %FileCheck %s --check-prefix=WASM-LOADER
// RUN: %run_if{wasm} %node %t/w 2>&1 | %FileCheck %s --check-prefix=EXEC
// RUN: %run_if{js} test -f %t/j
// RUN: %run_if{js} cat %t/j | %FileCheck %s --check-prefix=JS-OUTPUT
// RUN: %run_if{js} %node %t/j 2>&1 | %FileCheck %s --check-prefix=EXEC
```

## Readability Score (Subjective)

Based on clarity and ease of understanding:

1. **Strategy 3** (9/10) - Most intuitive, reads like natural language
2. **Strategy 5** (8/10) - Clear intent, familiar patterns
3. **Strategy 4** (8/10) - Very clean, but requires understanding parameterization
4. **Strategy 2** (7/10) - Clear, but macro syntax may be unfamiliar
5. **Strategy 1** (7/10) - Simple, but REQUIRES placement can be awkward
6. **Original** (4/10) - Cluttered with shell conditionals

## Maintainability Score

Based on ease of making changes:

1. **Strategy 3** (9/10) - Changes isolated to helper script
2. **Strategy 5** (8/10) - Clear patterns, easy to extend
3. **Strategy 2** (7/10) - Need to understand regex substitutions
4. **Strategy 4** (7/10) - Powerful but requires lit knowledge
5. **Strategy 1** (6/10) - May need to reorganize test files
6. **Original** (5/10) - Repetitive patterns prone to copy-paste errors

## Learning Curve

Time to understand for someone familiar with lit basics:

- **Strategy 1**: 5 minutes ⭐⭐⭐⭐⭐
- **Strategy 5**: 10 minutes ⭐⭐⭐⭐
- **Strategy 2**: 15 minutes ⭐⭐⭐
- **Strategy 3**: 20 minutes ⭐⭐⭐
- **Strategy 4**: 30-45 minutes ⭐⭐

## Summary

Choose based on your priorities:

- **Conciseness**: Strategy 3 or 4
- **Readability**: Strategy 3 or 5
- **Simplicity**: Strategy 1 or 5
- **Flexibility**: Strategy 2 or 3
- **Scalability**: Strategy 4
- **Easy Migration**: Strategy 5
