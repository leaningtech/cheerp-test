// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: Virtual calls in constructor 1/3 : SUCCESS
// CHECK: Virtual calls in constructor 2/3 : SUCCESS
// CHECK: Virtual calls in constructor 3/3 : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2018 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>

struct A
{
	double x;
	A():x(f())
	{
		assertEqual(x, 42.0, "Virtual calls in constructor 1/3");
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
		assertEqual(b, 43, "Virtual calls in constructor 2/3");
	}
	virtual int f()
	{
		return 43;
	}
};

void webMain()
{
	B* b = new B();
	assertEqual(b->b, 43, "Virtual calls in constructor 3/3");
}
