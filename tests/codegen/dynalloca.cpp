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
	//Test dynamically sized allocas
	volatile int size = 10;
	int sum = 0;
	if (size > 4)
	{
		int arr[size];
		for (int i = 0; i < size; i++)
		{
			arr[i] = i;
		}
		for (int i = 0; i < size; i++)
		{
			sum += arr[i];
		}
	}

	assertPrint("Dynamic alloca support 1/1:", sum);
	//CHECK: Dynamic alloca support 1/1: 45
}
