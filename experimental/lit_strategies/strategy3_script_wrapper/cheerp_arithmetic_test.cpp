// Strategy 3: Shell script wrapper approach
// This uses a helper script to handle conditional compilation and testing
// Most flexible but requires an external script

// RUN: mkdir -p %t
// RUN: %test_helper --compile wasm,js --output %t --source %s
// RUN: %test_helper --verify wasm --output %t --check-magic
// RUN: %test_helper --verify wasm --output %t --check-loader
// RUN: %test_helper --verify js --output %t --check-output
// RUN: %test_helper --execute wasm,js --output %t | %FileCheck %s --check-prefix=EXEC

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
    
    // Test modulo and comparison
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
