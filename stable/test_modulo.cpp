// Test modulo and comparison operations across all targets
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
// RUN: run_if_wasm %node %t/w 2>&1 | %FileCheck %s --check-prefix=MOD
// RUN: run_if_js %node %t/j 2>&1 | %FileCheck %s --check-prefix=MOD
// RUN: run_if_asmjs %node %t/a 2>&1 | %FileCheck %s --check-prefix=MOD
//
// PRE: Preexecution compile successful

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
