// Strategy 4: Loop-based approach with wrapper script
// This runs the test once for each enabled target
// Note: All targets run in a single test. For truly separate results,
// use separate test files or Strategy 1's REQUIRES directive.

// RUN: mkdir -p %t
// RUN: %run_all_targets %s %t | %FileCheck %s --check-prefix=EXEC

// Test basic arithmetic operations and printf functionality
#include <stdio.h>

int main() {
    // Test basic arithmetic
    int addition = 8 + 12;
    int subtraction = 25 - 5;
    int multiplication = 4 * 3;
    int division = 24 / 6;
    
    printf("Addition: %d\n", addition);
    printf("Subtraction: %d\n", subtraction);
    printf("Multiplication: %d\n", multiplication);
    printf("Division: %d\n", division);
    
    int modulo = 17 % 5;
    int is_equal = (addition == 20) ? 1 : 0;
    
    printf("Modulo: %d\n", modulo);
    printf("Equal check: %d\n", is_equal);
    
    return 0;
}

// EXEC: Addition: 20
// EXEC: Subtraction: 20
// EXEC: Multiplication: 12
// EXEC: Division: 4
// EXEC: Modulo: 2
// EXEC: Equal check: 1
