// RUN: mkdir -p %t
//
// Preexecution mode (PREEXECUTE_MODE=true):
// RUN: preexec_only run_if_js %valgrind compile_mode_js -o %t/test_preexec_js %s 2>&1 | %Filecheck %s --check-prefix=PREEXEC_OK
// RUN: preexec_only run_if_asmjs %valgrind compile_mode_asmjs -o %t/test_preexec_asmjs %s 2>&1 | %Filecheck %s --check-prefix=PREEXEC_OK
//
// Regular mode (PREEXECUTE_MODE=false):
// RUN: regular_only run_if_js %valgrind compile_mode_js -o %t/test_regular_js %s 2>&1 && node %t/test_regular_js 2>&1 | %FileCheck %s --check-prefix=REG_OK
// RUN: regular_only run_if_asmjs %valgrind compile_mode_asmjs -o %t/test_regular_asmjs %s 2>&1 && node %t/test_regular_asmjs 2>&1 | %FileCheck %s --check-prefix=REG_OK
// RUN: regular_only run_if_wasm %valgrind compile_mode_wasm -o %t/test_regular_wasm %s 2>&1 && node %t/test_regular_wasm 2>&1 | %FileCheck %s --check-prefix=REG_OK


// PREEXEC_OK-NOT: error
// REG_OK : Simple addition : SUCCESS
// REG_OK: Computed value : SUCCESS
// REG_OK: Negative condition : SUCCESS

#include <tests.h>

int main() {
    // Test 1: Simple compile-time constant arithmetic
    // This should pass in both regular and preexecution mode
    assertEqual(2 + 3, 5, "Simple addition");
    
    // Test 2: Slightly more complex computation
    // This should also pass - demonstrates preexecuter can handle expressions
    int computed = 10 * 2 + 5;
    assertEqual(computed, 25, "Computed value");
    
    // Test 3: Logical operation
    // This demonstrates boolean evaluation at compile time
    bool condition = (10 > 5) && (3 < 7);
    assertEqual(condition, true, "Negative condition");
    
    return 0;
}
  