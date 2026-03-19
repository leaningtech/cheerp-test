//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: mkdir -p %t

// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1| %FileCheck %s
// RUN: preexec_only run_if_js compile_mode_js -o %t/j %s 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 | %FileCheck %s

#include <tests.h>

struct A;

struct C
{
	A* a;
};

struct B
{
	C c;
};

struct A
{
	B b;
};

void webMain()
{
	// Test collapsing of recursive struct
	A a;
	a.b.c.a = &a;
	// Do a cast that is only valid if the struct is collapsed
	B* b = reinterpret_cast<B*>(&a);

	assertPrint("Collapsing of recursive structs:", (signed int)(b->c.a == &a));
	// CHECK: Collapsing of recursive structs: 1
}
