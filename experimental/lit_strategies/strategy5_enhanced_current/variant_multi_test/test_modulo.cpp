// Test modulo and comparison operations across all targets
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
// RUN: %run_if{wasm} %node %t/w 2>&1 | %FileCheck %s --check-prefix=MOD
// RUN: %run_if{js} %node %t/j 2>&1 | %FileCheck %s --check-prefix=MOD
// RUN: %run_if{asmjs} %node %t/a 2>&1 | %FileCheck %s --check-prefix=MOD

#include <stdio.h>

int main() {
    int modulo = 17 % 5;
    int is_equal = (20 == 20) ? 1 : 0;
    int is_not_equal = (10 != 20) ? 1 : 0;
    
    printf("Modulo: %d\n", modulo);
    printf("Equal check: %d\n", is_equal);
    printf("Not equal check: %d\n", is_not_equal);
    
    return 0;
}

// MOD: Modulo: 2
// MOD: Equal check: 1
// MOD: Not equal check: 1
