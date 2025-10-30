// XFAIL: *
// NOTE: This test requires .testing.js harness infrastructure not yet implemented in lit

//===---------------------------------------------------------------------===//
//	Copyright 2018 Leaning Technlogies
//===----------------------------------------------------------------------===//

// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: Virtual calls in both modes 1/2 : SUCCESS
// CHECK: Virtual calls in both modes 2/2 : SUCCESS

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
	assertEqual(runGeneric(a), 43, "Virtual calls in both modes 1/2");
	assertEqual(runWasm(c), 45, "Virtual calls in both modes 2/2");
}
