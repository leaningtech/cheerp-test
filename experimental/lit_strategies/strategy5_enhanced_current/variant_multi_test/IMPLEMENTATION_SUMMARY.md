# Strategy 5 Enhanced Current - Multi-Test Variant Implementation

## Overview

Successfully adapted the Strategy 4 loop-based tests to Strategy 5's enhanced macro approach. This demonstrates how to handle multiple test files with explicit per-target compilation and execution using clean macro syntax.

## Files Created

1. **lit.cfg** - Configuration with macro definitions
   - `%compile_for{target}` - Substitutes target-specific compiler command
   - `%run_if{target}` - Conditional prefix that strips away when enabled, becomes `true` when disabled

2. **test_arithmetic.cpp** - Tests basic arithmetic operations
   - Compiles for all three targets (wasm, js, asmjs)
   - Runs output verification with FileCheck for each enabled target
   - Uses `--check-prefix=ARITH` to distinguish from other tests

3. **test_modulo.cpp** - Tests modulo and comparison operations
   - Same structure as test_arithmetic
   - Uses `--check-prefix=MOD` for output verification

4. **README.md** - Comprehensive documentation
   - Usage examples
   - Macro explanation
   - Troubleshooting guide
   - Comparison with Strategy 4

## Test Results

### Test Run 1: Single Target (JS)
```
Command: lit . --param TARGETS=js
Result: PASS - 2 tests passed in 1.84s
- test_modulo.cpp: PASS
- test_arithmetic.cpp: PASS
```

### Test Run 2: Multiple Targets (WASM, JS)
```
Command: lit . --param TARGETS=wasm,js
Result: PASS - 2 tests passed in 4.22s
- Both tests executed for both targets
- All output matches FileCheck patterns
```

### Test Run 3: Single Test, Single Target (test_arithmetic.cpp with WASM)
```
Command: lit test_arithmetic.cpp --param TARGETS=wasm
Result: PASS - 1 test passed in 2.28s
- Successfully compiled and executed WASM target
- Output verified with FileCheck
```

## Macro Expansion Verification

Examining the generated test script confirms proper macro substitution:

**For WASM enabled:**
- Line 2: `%compile_for{wasm} -O1 -o %t/w %s` 
  → `/opt/cheerp/bin/clang++ -target cheerp-wasm -O1 -o /path/w /path/test.cpp`
- Line 5: `%run_if{wasm} test -f %t/w && test -f %t/w.wasm`
  → `test -f /path/w && test -f /path/w.wasm`
- Line 8: `%run_if{wasm} %node %t/w 2>&1 | %FileCheck ...`
  → `node /path/w 2>&1 | /usr/bin/FileCheck-20 ... --check-prefix=ARITH`

**For JS/ASMJS disabled:**
- `%compile_for{js}` → `true`
- `%compile_for{asmjs}` → `true`
- `%run_if{js}` ... → `true`
- `%run_if{asmjs}` ... → `true`

## Key Differences from Strategy 4 Variant

| Aspect | Strategy 4 | Strategy 5 |
|--------|-----------|-----------|
| **Implementation** | Wrapper shell script | Pure lit substitutions |
| **RUN lines per test** | Single line (uses wrapper) | 15-18 lines (explicit per-target) |
| **Test structure** | Abstracted in script | Visible in test file |
| **Debugging** | See wrapper script behavior | See expanded RUN commands directly |
| **Migration** | More complex | Simpler (search/replace patterns) |

## How This Variant Works

### 1. Configuration (lit.cfg)
- Reads `TARGETS` parameter from command line
- Creates substitution patterns for each target
- Enabled targets get full compiler command substitution
- Disabled targets get `true` (no-op) substitution

### 2. Test Files
- Use `%compile_for{target}` to compile for specific target
- Use `%run_if{target}` prefix for conditional execution
- Each target's actions are explicit and visible
- FileCheck patterns distinguish outputs per target

### 3. Execution Flow
When running `lit . --param TARGETS=wasm,js`:

1. lit.cfg recognizes wasm and js as enabled targets
2. `%compile_for{wasm}` → full compiler command
3. `%compile_for{js}` → full compiler command  
4. `%compile_for{asmjs}` → `true`
5. All `%run_if{wasm}` lines execute normally
6. All `%run_if{js}` lines execute normally
7. All `%run_if{asmjs}` lines become `true` (no-op)

## Advantages Over Strategy 4

1. **No external scripts** - Everything in lit.cfg and test files
2. **Standard lit syntax** - Uses lit's built-in substitution mechanism
3. **Incremental adoption** - Can migrate existing tests one-by-one
4. **Clear test logic** - Each target's actions are visible in the test
5. **Easier debugging** - Can inspect expanded RUN commands directly

## Advantages Over Strategy 5 Single-Test Version

1. **Better organization** - Multiple test files for different concerns
2. **Focused tests** - Each file tests specific functionality
3. **Parallel-friendly** - Tests can run in parallel (test_arithmetic and test_modulo)
4. **Scalable** - Easy to add new test files
5. **Maintainable** - Clear separation of test concerns

## When to Use This Variant

Best for projects that:
- Have multiple related but independent test cases
- Want clean, readable test structure
- Need to migrate from original approach with minimal disruption
- Prefer explicit per-target handling
- Want to avoid external helper scripts

## Testing Recommendations

1. **Run all targets:**
   ```bash
   lit .
   ```

2. **Run specific target subset:**
   ```bash
   lit . --param TARGETS=wasm,js
   ```

3. **Run single test with verbose output:**
   ```bash
   lit test_arithmetic.cpp -vv
   ```

4. **Add new test file:**
   - Copy test_arithmetic.cpp as template
   - Update test logic and FileCheck patterns
   - Use same macro syntax

## Future Enhancements

Possible extensions to this variant:

1. **Shared utilities** - Create a `tests.h` for common test functions
2. **More test files** - Add tests for loops, functions, classes, etc.
3. **Complex verification** - Add tests with multiple FileCheck passes
4. **Performance tests** - Add timing comparisons between targets
5. **Integration tests** - Test interaction between multiple features

## Conclusion

This Strategy 5 variant successfully demonstrates:
- ✅ Clean, readable macro-based test syntax
- ✅ Support for multiple test files
- ✅ Explicit per-target handling
- ✅ Proper macro expansion and substitution
- ✅ Cross-target test execution
- ✅ Easy migration path from original approach
