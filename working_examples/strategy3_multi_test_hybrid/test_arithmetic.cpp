// Test basic arithmetic operations across all targets
// Using Strategy 5 enhanced macro approach
//
// RUN: mkdir -p %t
// Preexecution mode (PREEXECUTE_MODE=true):
// RUN: compile_for_preexec_js -O1 -o %t/j_pre %s 2>&1 && echo "Preexecution compile successful" | %FileCheck %s --check-prefix=PRE
// RUN: compile_for_preexec_asmjs -O1 -o %t/a_pre %s 2>&1 && echo "Preexecution compile successful" | %FileCheck %s --check-prefix=PRE
//
// Regular mode (PREEXECUTE_MODE=false):
// RUN: compile_for_wasm -O1 -o %t/w %s
// RUN: compile_for_js -O1 -o %t/j %s
// RUN: compile_for_asmjs -O1 -o %t/a %s
//
// RUN: run_if_wasm test -f %t/w && test -f %t/w.wasm
// RUN: run_if_js test -f %t/j
// RUN: run_if_asmjs test -f %t/a
//
// RUN: run_if_wasm %node %t/w 2>&1 | %FileCheck %s --check-prefix=ARITH
// RUN: run_if_js %node %t/j 2>&1 | %FileCheck %s --check-prefix=ARITH
// RUN: run_if_asmjs %node %t/a 2>&1 | %FileCheck %s --check-prefix=ARITH
//
// PRE: Preexecution compile successful

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
