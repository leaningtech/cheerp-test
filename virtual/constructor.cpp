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
	double x;
	A():x(f())
	{
		assertPrint("Virtual calls in constructor 1/3:", x);
		// CHECK: Virtual calls in constructor 1/3: 42.0000
	}
	virtual int f()
	{
		return 42;
	}
};

struct B: public A
{
	int b;
	B():b(f())
	{
		assertPrint("Virtual calls in constructor 2/3:", b);
		// CHECK: Virtual calls in constructor 2/3: 43 
	}
	virtual int f()
	{
		return 43;
	}
};

void webMain()
{
	B* b = new B();
	assertPrint("Virtual calls in constructor 3/3:", b->b);
	// CHECK: Virtual calls in constructor 3/3: 43
}
