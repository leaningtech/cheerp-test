//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_js compile_mode_js -o %t/j %s 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 | %FileCheck %s


#include <tests.h>

void webMain()
{
	// Test dynamic stack allocation
	volatile int length = 100;
	double v1[length];
	for(int i=0;i<length;i++)
		v1[i] = double(i)/2;
	assertPrint("Dyn stack 1/2:", v1[1]);
	// CHECK: Dyn stack 1/2: 0.5

	double* v2 = static_cast<double*>(alloca(length*sizeof(double)));
	for(int i=0;i<length;i++)
		v2[i] = double(i)/2;
	assertPrint("Dyn stack 2/2:", v2[1]);
	// CHECK: Dyn stack 2/2: 0.5
}
