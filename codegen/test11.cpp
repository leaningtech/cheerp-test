//===---------------------------------------------------------------------===//
//      Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_js compile_mode_js -o %t/j %s 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 | %FileCheck %s

// CHECK: GEP aliasing: 44 

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
	assertPrint("GEP aliasing:", (int)p3);
}
