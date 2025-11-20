// Example test for preexecution mode in strategy1
// This test is designed to work with both regular and preexecution modes
// RUN: mkdir -p %t
// RUN: %run_all_targets %s %t 2>&1 | %FileCheck %s
// CHECK: SUCCESS
// CHECK: SUCCESS
// CHECK: SUCCESS

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
    
    // Test basic arithmetic
    assertEqual(add(2, 3), 5, "Addition");
    
    // Test multiplication
    assertEqual(multiply(4, 5), 20, "Multiplication");
    
    // Test boolean logic
    assertEqual(isEven(42), true, "Even number check");
    
    return 0;
}
