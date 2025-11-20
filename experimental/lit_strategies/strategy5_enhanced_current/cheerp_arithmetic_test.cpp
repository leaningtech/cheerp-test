// Strategy 5: Enhanced version of the current approach
// This improves on the original with better substitution patterns
// that reduce verbosity while maintaining the same conditional logic

// RUN: mkdir -p %t
// RUN: %compile_for{wasm} -o %t/w %s
// RUN: %compile_for{js} -o %t/j %s

// RUN: %run_if{wasm} test -f %t/w.wasm && test -f %t/w
// RUN: %run_if{wasm} xxd -l 8 %t/w.wasm | %FileCheck %s --check-prefix=WASM-MAGIC
// RUN: %run_if{wasm} cat %t/w | %FileCheck %s --check-prefix=WASM-LOADER
// RUN: %run_if{wasm} %node %t/w 2>&1 | %FileCheck %s --check-prefix=EXEC

// RUN: %run_if{js} test -f %t/j
// RUN: %run_if{js} cat %t/j | %FileCheck %s --check-prefix=JS-OUTPUT
// RUN: %run_if{js} %node %t/j 2>&1 | %FileCheck %s --check-prefix=EXEC

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
