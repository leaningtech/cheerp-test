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

struct B;

struct A
{
	A* a;
	A* (*func)(A*);
};

struct B
{
	B* b;
	A* (*func)(A*);
};

A* funcDef(A*)
{
}

A globalA = { NULL, funcDef };

void webMain()
{
	A a;
	a.a = &a;
	volatile B b;
	assertPrint("Recursive structure uses:", (signed int)(a.a == &a));
	// CHECK: Recursive structure uses: 1
}
