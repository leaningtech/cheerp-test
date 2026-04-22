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

void webMain()
{
	//Test pointer arithmetic
	int a[6] = { 1, 2, 3, 4, 5, 6};

	int* volatile a1=&a[1];
	int* volatile a2=&a[5];
	assertPrint("Pointer subtraction:", a2 - a1);
	// CHECK: Pointer subtraction: 4

	int* volatile a3=a+2;
	assertPrint("Pointer subtraction:", a3 - a);
	// CHECK: Pointer subtraction: 2
}
