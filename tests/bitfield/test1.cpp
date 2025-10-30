//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_js compile_mode_js -o %t/j %s 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 | %FileCheck %s



#include "./../tests.h"
struct A
{
	short a1:15;
	short a2:2;
	int a3:10;
};

void webMain()
{
	A a;
	a.a1=7;
	a.a2=1;
	a.a3=511;
	assertPrint("Bitfield test 1/3:", a.a1);
	assertPrint("Bitfield test 2/3:", a.a2);
	assertPrint("Bitfield test 2/3:", a.a3);
}

// CHECK: Bitfield test 1/3: 7
// CHECK: Bitfield test 2/3: 1
// CHECK: Bitfield test 2/3: 511
