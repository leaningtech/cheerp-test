// RUN: %if_js %cheerp_clang -O1 -frtti -I%S/.. -target cheerp %s -o %t.js %endif
// RUN: %if_js %node %t.js 2>&1 | %FileCheck %s %endif
// RUN: %if_asmjs %cheerp_clang -O1 -frtti -I%S/.. -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js %endif
// RUN: %if_asmjs %node %t_asmjs.js 2>&1 | %FileCheck %s %endif
// RUN: %if_wasm %cheerp_clang -O1 -frtti -I%S/.. -target cheerp-wasm %s -o %t_wasm.js %endif
// RUN: %if_wasm %node %t_wasm.js 2>&1 | %FileCheck %s %endif
// CHECK: Pointer subtraction : SUCCESS
// CHECK: Pointer addition : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2013 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>

void webMain()
{
	//Test pointer arithmetic
	int a[6] = { 1, 2, 3, 4, 5, 6};

	int* volatile a1=&a[1];
	int* volatile a2=&a[5];
	assertEqual(a2-a1, 4L, "Pointer subtraction");

	int* volatile a3=a+2;
	assertEqual(*a3, 3, "Pointer addition");
}
