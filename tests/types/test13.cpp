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

struct A
{
	int i;
};

struct B
{
	int i;
};

void webMain()
{
	volatile B b = { 2 };
	volatile A a = { 1 };
	volatile void * volatile p = &b.i;
	volatile int ai = a.i;
	volatile int bi = b.i;

	assertPrint("Pointers to member variables 1/2:", (int)ai);
	// CHECK: Pointers to member variables 1/2: 1
	assertPrint("Pointers to member variables 2/2:", int(bi));
	// CHECK: Pointers to member variables 2/2: 2
}
