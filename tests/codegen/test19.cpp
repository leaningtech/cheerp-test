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

int testSwitch(volatile int16_t* i)
{
	switch((int16_t)*i)
	{
		case (int16_t)10:
			return 0;
		case (int16_t)-1:
			return 1;
		default:
			return 2;
	}
}

void webMain()
{
	volatile int16_t a = 0xffff;
	assertPrint("Switch on 16-bit integers 1/3:", testSwitch(&a)); // CHECK: Switch on 16-bit integers 1/3: 1
	a = -1;
	assertPrint("Switch on 16-bit integers 2/3:", testSwitch(&a)); // CHECK: Switch on 16-bit integers 2/3: 1
	a = 10;
	assertPrint("Switch on 16-bit integers 3/3:", testSwitch(&a)); // CHECK: Switch on 16-bit integers 3/3: 0
}
