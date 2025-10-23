// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: expected p1 memory to be allocated : SUCCESS
// CHECK: expected p2 memory to be allocated : SUCCESS
// CHECK: expected p3 memory to be allocated : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2022 Leaning Technologies
//===----------------------------------------------------------------------===//
#include <tests.h>
#include <stdlib.h>

int* p1 = (int*) malloc(900);
int* p2 = (int*) calloc(1, 900);
int* p3 = (int*) realloc((int*) 0, 900);

int main() {
	assertEqual(p1!=0, true, "expected p1 memory to be allocated");
	assertEqual(p2!=0, true, "expected p2 memory to be allocated");
	assertEqual(p3!=0, true, "expected p3 memory to be allocated");
}
