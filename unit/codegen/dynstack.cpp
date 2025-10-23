// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: Dyn stack 1/2 : SUCCESS
// CHECK: Dyn stack 2/2 : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2019 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>

void webMain()
{
	// Test dynamic stack allocation
	volatile int length = 100;
	double v1[length];
	for(int i=0;i<length;i++)
		v1[i] = double(i)/2;
	assertEqual(v1[1], 0.5, "Dyn stack 1/2");

	double* v2 = static_cast<double*>(alloca(length*sizeof(double)));
	for(int i=0;i<length;i++)
		v2[i] = double(i)/2;
	assertEqual(v2[1], 0.5, "Dyn stack 2/2");
}
