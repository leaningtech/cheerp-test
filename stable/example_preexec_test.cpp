// Example test for strategy3 that works in both regular and preexecution modes
// This test demonstrates using compile_for_<target> and compile_for_preexec_<target> macros
// RUN: mkdir -p %t
// 
// Preexecution mode (PREEXECUTE_MODE=true):
// RUN: compile_for_preexec_js -o %t/test_preexec_js %s 2>&1 && echo "Preexecution compile successful"
// RUN: compile_for_preexec_asmjs -o %t/test_preexec_asmjs %s 2>&1 && echo "Preexecution compile successful"
//
// Regular mode (PREEXECUTE_MODE=false):
// RUN: compile_for_js -o %t/test_regular_js %s 2>&1 && node %t/test_regular_js 2>&1 | %FileCheck %s --check-prefix=REG
// RUN: compile_for_wasm -o %t/test_regular_wasm %s 2>&1 && node %t/test_regular_wasm 2>&1 | %FileCheck %s --check-prefix=REG
// RUN: compile_for_asmjs -o %t/test_regular_asmjs %s 2>&1 && node %t/test_regular_asmjs 2>&1 | %FileCheck %s --check-prefix=REG
//
// REG: Addition : SUCCESS
// REG: Multiplication : SUCCESS
// REG: Even number check : SUCCESS

#include <tests.h>

// Simple compile-time evaluable functions
int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

bool isEven(int n) {
    return (n % 2) == 0;
}

int main() {
    // These assertions will be evaluated at compile-time when PRE_EXECUTE_TEST is defined
    // At runtime otherwise
    
    assertEqual(add(2, 3), 5, "Addition");
    assertEqual(multiply(4, 5), 20, "Multiplication");
    assertEqual(isEven(42), true, "Even number check");
    
    return 0;
}
