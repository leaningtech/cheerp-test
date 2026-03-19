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
#include <math.h>

union U
{
	double d;
	uint64_t u64;
};

void webMain()
{
	//Test union with uint64_t
	U u;
	u.d = M_PI;
	assertPrint("Access to union with uint64_t member 1/2:", (uint64_t)u.u64);
	// CHECK: Access to union with uint64_t member 1/2: 4614256656552045848
	// assertEqual<uint64_t>(u.u64, 4614256656552045848, "Access to union with unt64_t member 1/2");
	u.d = 0.0;
	u.u64 = 4614256656552045848;
	// assertEqual<double>(u.d, M_PI, "Access to union with unt64_t member 2/2");
	assertPrint("Access to union with uint64_t member 2/2:", (signed int)((double)u.d == M_PI));
	// CHECK: Access to union with uint64_t member 2/2: 1
}

