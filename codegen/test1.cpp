//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_js compile_mode_js -o %t/j %s 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 | %FileCheck %s

// CHECK: PHI support 1/2: 2
// CHECK: PHI support 2/2: 1

#include <tests.h>

void webMain()
{
	//Test recursive dependency in PHIs
	int a = 1;
	int b = 2;

	for(volatile int i=0;i<3;i++)
	{
		int c=a;
		a=b;
		b=c;
	}

	assertPrint("PHI support 1/2:", a);
	assertPrint("PHI support 2/2:", b);
}
