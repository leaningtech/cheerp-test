//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// XFAIL:*

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1| %FileCheck %s
// RUN: preexec_only run_if_js compile_mode_js -o %t/j %s 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 | %FileCheck %s

#include <tests.h>

struct [[cheerp::genericjs]] A
{
	virtual int f()
	{
		return 42;
	}
	virtual ~A()
	{
	}
};

struct [[cheerp::genericjs]] B: public A
{
	virtual int f()
	{
		return 43;
	}
};

struct [[cheerp::wasm]] C
{
	virtual int f()
	{
		return 44;
	}
	virtual ~C()
	{
	}
};

struct [[cheerp::wasm]] D: public C
{
	virtual int f()
	{
		return 45;
	}
};

[[cheerp::genericjs]] int runGeneric(A* a)
{
	return a->f();
}

[[cheerp::wasm]] int runWasm(C* c)
{
	return c->f();
}

void webMain()
{
	B b;
	D d;
	A* volatile a = &b;
	C* volatile c = &d;
	assertEqual(43, runGeneric(a), "virtual calls in both modes 1/2:");
	// CHECK: Virtual calls in both modes 1/2: 1
	assertEqual(45, runWasm(c), "Virtual calls in both modes 2/2:");
	// CHECK: Virtual calls in both modes 2/2: 1
}
