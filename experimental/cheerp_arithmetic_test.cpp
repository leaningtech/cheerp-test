// RUN: mkdir -p %t
// RUN: %cheerp_clang -target cheerp-wasm -o %t/w %s
// RUN: %cheerp_clang -target cheerp -o %t/j %s
// RUN: %node %t/w 2>&1 | %FileCheck %s -vv --check-prefix=EXEC_WASM
// RUN: %node %t/j 2>&1 | %FileCheck %s -vv --check-prefix=EXEC_JS

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


// EXEC_WASM: Addition: 20
// EXEC_WASM: Subtraction: 20
// EXEC_WASM: Multiplication: 12
// EXEC_WASM: Division: 4
// EXEC_WASM: Modulo: 2
// EXEC_WASM: Equal check: 1

// EXEC_JS: Addition: 20
// EXEC_JS: Subtraction: 20
// EXEC_JS: Multiplication: 12
// EXEC_JS: Division: 4
// EXEC_JS: Modulo: 2
// EXEC_JS: Equal check: 1