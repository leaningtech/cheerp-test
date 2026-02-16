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

union U
{
	struct { double x, y, z; };
	double a[3];
};

struct A
{
	U u;
};

// Partial global initialization to test TypeOptimizer
U globalU = { 42, 43 };

void webMain()
{
	// Force globalU linking
	U* volatile gU = &globalU;

	//Test struct in union
	volatile U u;
	u.a[0] = 42;
	u.a[1] = 43;
	u.a[2] = 44;
	assertPrint("Struct in unions 1/6:", (double)u.x);
	// CHECK: Struct in unions 1/6: 42.000
	assertPrint("Struct in unions 2/6:", (double)u.y);
	// CHECK: Struct in unions 2/6: 43.000
	assertPrint("Struct in unions 3/6:", (double)u.z);
	// CHECK: Struct in unions 3/6: 44.000

	volatile A a;
	a.u.a[0] = 45;
	a.u.a[1] = 46;
	a.u.a[2] = 47;
	assertPrint("Struct in unions 1/6:", (double)a.u.x);
	// CHECK: Struct in unions 1/6: 45.000
	assertPrint("Struct in unions 2/6:", (double)a.u.y);
	// CHECK: Struct in unions 2/6: 46.000
	assertPrint("Struct in unions 3/6:", (double)a.u.z);
	// CHECK: Struct in unions 3/6: 47.000

	volatile double* volatile f = &u.a[1];
	assertPrint("Struct in unions 1/3:", (double)f[-1]);
	// CHECK: Struct in unions 1/3: 42.000
	assertPrint("Struct in unions 2/3:", (double)f[0]);
	// CHECK: Struct in unions 2/3: 43.000	
	assertPrint("Struct in unions 3/3:", (double)f[1]);
	// CHECK: Struct in unions 3/3: 44.000
}
