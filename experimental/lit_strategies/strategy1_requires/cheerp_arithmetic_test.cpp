// Strategy 1: Using REQUIRES directive with lit features
// This is the simplest approach - tests are filtered at the lit level
// Each RUN line only executes if the required feature is available

// RUN: mkdir -p %t
// RUN: %cheerp_clang -target cheerp-wasm -o %t/w %s
// RUN: %cheerp_clang -target cheerp -o %t/j %s

// WASM tests - only run if 'wasm' feature is available
// REQUIRES: wasm
// RUN: test -f %t/w.wasm && test -f %t/w
// RUN: xxd -l 8 %t/w.wasm | %FileCheck %s --check-prefix=WASM-MAGIC
// RUN: cat %t/w | %FileCheck %s --check-prefix=WASM-LOADER
// RUN: %node %t/w 2>&1 | %FileCheck %s --check-prefix=EXEC

// JS tests - only run if 'js' feature is available
// REQUIRES: js
// RUN: test -f %t/j
// RUN: cat %t/j | %FileCheck %s --check-prefix=JS-OUTPUT
// RUN: %node %t/j 2>&1 | %FileCheck %s --check-prefix=EXEC

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

// WASM-MAGIC: 0061 736d
// WASM-LOADER: WebAssembly
// JS-OUTPUT: {{(printf|console)}}
// EXEC: Addition: 20
// EXEC: Subtraction: 20
// EXEC: Multiplication: 12
// EXEC: Division: 4
// EXEC: Modulo: 2
// EXEC: Equal check: 1
