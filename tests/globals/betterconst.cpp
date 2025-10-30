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

struct A
{
	char c;
};

struct B: public A
{
	float f;
};


B b;

A* volatile a1 = &b;

char* volatile c1 = &b.c;

struct C
{
	A a;
	float f;
};

C c = { { 'A' }, 42.0f };

A* volatile a2 = &c.a;

char* volatile c2 = &c.a.c;

float* volatile f2 = &c.f;

void* volatile v = &b;

void webMain()
{
	b.c = 'A';
	b.f = 42.0f;	

	assertPrint("Better generation of constant init for globals 1/6:", a1->c);
	// CHECK: Better generation of constant init for globals 1/6: A
	assertPrint("Better generation of constant init for globals 2/6:", *c1);
	// CHECK: Better generation of constant init for globals 2/6: A
	assertPrint("Better generation of constant init for globals 3/6:", a2->c);
	// CHECK: Better generation of constant init for globals 3/6: A
	assertPrint("Better generation of constant init for globals 4/6:", *c2);
	// CHECK: Better generation of constant init for globals 4/6: A
	assertPrint("Better generation of constant init for globals 5/6:", *f2);
	// CHECK: Better generation of constant init for globals 5/6: 42.0
	assertPrint("Better generation of constant init for globals 6/6:", ((B*)v)->f);
	// CHECK: Better generation of constant init for globals 6/6: 42.0
}






