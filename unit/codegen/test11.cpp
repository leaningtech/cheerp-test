// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: GEP aliasing : SUCCESS

//===---------------------------------------------------------------------===//
//      Copyright 2014 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>

struct A
{
	int p;
};

int g = 43;

int* __attribute__((noinline)) selectorFunc(A* a, int d)
{
	if(d)
		return &g;
	else
		return &a->p;
}

void webMain()
{
	A a;
	a.p = 42;
	volatile bool cond=false;
	int* p2 = 0;
	if(cond)
		p2 = selectorFunc(&a, 1);
	else
		p2 = selectorFunc(&a, 0);
	a.p = 44;
	volatile int p3 = *p2;
	assertEqual((int)p3, 44, "GEP aliasing");
}
