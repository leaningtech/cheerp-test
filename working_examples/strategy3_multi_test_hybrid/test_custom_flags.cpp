// Test to verify custom compilation flags
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
// RUN: run_if_wasm %node %t/w 2>&1 | %FileCheck %s --check-prefix=CHECK
// RUN: run_if_js %node %t/j 2>&1 | %FileCheck %s --check-prefix=CHECK
// RUN: run_if_asmjs %node %t/a 2>&1 | %FileCheck %s --check-prefix=CHECK
//
// PRE: Preexecution compile successful
// CHECK: Custom flags test

#include <stdio.h>

int main() {
#ifdef NDEBUG
    printf("Custom flags test: NDEBUG defined\n");
#else
    printf("Custom flags test: Debug mode\n");
#endif
    return 0;
}
