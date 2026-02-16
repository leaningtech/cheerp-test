//===----------------------------------------------------------------------===//
//      Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_js compile_mode_js -o %t/j %s 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 | %FileCheck %s


#include <tests.h>

struct A
{
	int i;
};

struct B
{
	A* a;
};

void webMain()
{
	volatile B b;
	b.a = new A;
	b.a->i = 42;
	volatile A* a2 = new A[10];
	for(int i=0;i<10;i++)
		a2[i].i = i;
	assertPrint("Accessing member of stack allocated struct:", b.a->i);
	// CHECK: Accessing member of stack allocated struct: 42

	assertPrint("Accessing member of heap allocated struct:", (int)a2[8].i);
	// CHECK: Accessing member of heap allocated struct: 8
	
}
