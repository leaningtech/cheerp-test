// Example test for strategy2 that works in both regular and preexecution modes
// REQUIRES: js
// RUN: mkdir -p %t
// 
// Preexecution mode (PREEXECUTE_MODE=true):
// REQUIRES: preexecute
// RUN: %cheerp_clang_preexec -O1 -frtti -I%p -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp -o %t/test_preexec_js %s 2>&1 && echo "Preexecution assertions evaluated at compile-time: SUCCESS for js" | %FileCheck %s --check-prefix=PRE
// 
// Regular mode (default):
// RUN: %cheerp_clang -O1 -frtti -I%p -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp -o %t/test_regular_js %s 2>&1 && node %t/test_regular_js 2>&1 | %FileCheck %s --check-prefix=REG
// 
// PRE: Preexecution assertions evaluated at compile-time: SUCCESS for js
// REG: SUCCESS
// REG: SUCCESS
// REG: SUCCESS

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
    
    assertEqual(add(2, 3), 5, "Addition");
    assertEqual(multiply(4, 5), 20, "Multiplication");
    assertEqual(isEven(42), true, "Even number check");
    
    return 0;
}
