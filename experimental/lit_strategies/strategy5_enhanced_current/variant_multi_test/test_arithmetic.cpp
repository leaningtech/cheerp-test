// Test basic arithmetic operations across all targets
// Using Strategy 5 enhanced macro approach

// RUN: mkdir -p %t
// RUN: %compile_for{wasm} -O1 -o %t/w %s
// RUN: %compile_for{js} -O1 -o %t/j %s
// RUN: %compile_for{asmjs} -O1 -o %t/a %s
//
// RUN: %run_if{wasm} test -f %t/w && test -f %t/w.wasm
// RUN: %run_if{js} test -f %t/j
// RUN: %run_if{asmjs} test -f %t/a
//
// RUN: %run_if{wasm} %node %t/w 2>&1 | %FileCheck %s --check-prefix=ARITH
// RUN: %run_if{js} %node %t/j 2>&1 | %FileCheck %s --check-prefix=ARITH
// RUN: %run_if{asmjs} %node %t/a 2>&1 | %FileCheck %s --check-prefix=ARITH

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

// ARITH: Addition: 20
// ARITH: Subtraction: 20
// ARITH: Multiplication: 12
// ARITH: Division: 4
