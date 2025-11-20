// Test basic arithmetic operations across all targets
// RUN: mkdir -p %t
// RUN: %run_all_targets %s %t | %FileCheck %s --check-prefix=CHECK

#include <stdio.h>

int main() {
    int addition = 8 + 12;
    int subtraction = 25 - 5;
    int multiplication = 4 * 3;
    int division = 24 / 6;
    
    printf("Addition: %d\n", addition);
    printf("Subtraction: %d\n", subtraction);
    printf("Multiplication: %d\n", multiplication);
    printf("Division: %d\n", division);
    
    return 0;
}

// CHECK: Addition: 20
// CHECK: Subtraction: 20
// CHECK: Multiplication: 12
// CHECK: Division: 4
