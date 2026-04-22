//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t

// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1| %FileCheck %s
// RUN: preexec_only run_if_js compile_mode_js -o %t/j %s 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 | %FileCheck %s

#include <tests.h>
#include <stdlib.h>

int* p1 = (int*) malloc(900);
int* p2 = (int*) calloc(1, 900);
int* p3 = (int*) realloc((int*) 0, 900);

int main() {
	assertPrint("expected p1 memory to be allocated:", (p1 != 0));
	// CHECK: expected p1 memory to be allocated: 
	// CHECK-NOT: 0
	assertPrint("expected p2 memory to be allocated:", (p2 != 0));
	// CHECK: expected p2 memory to be allocated:
	// CHECK-NOT: 0
	assertPrint("expected p3 memory to be allocated:", (p3 != 0));
	// CHECK: expected p3 memory to be allocated:
	// CHECK-NOT: 0
}
