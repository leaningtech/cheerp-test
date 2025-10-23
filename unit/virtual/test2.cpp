// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: Complex virtual call 1/5 : SUCCESS
// CHECK: Complex virtual call 2/5 : SUCCESS
// CHECK: Complex virtual call 3/5 : SUCCESS
// CHECK: Complex virtual call 4/5 : SUCCESS
// CHECK: Complex virtual call 5/5 : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2013 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>

struct A
{
	int a;
	virtual int f()
	{
		return 1;
	}
	virtual ~A()
	{
	}
};

struct B: public A
{
	virtual int f()
	{
		return 2;
	}
	virtual ~B()
	{
	}
};

struct C: public B
{
	virtual int f()
	{
		return 3;
	}
	virtual ~C()
	{
	}
};

void webMain()
{
	C c;
	C* volatile c2=&c;
	B* volatile b=c2;
	A* volatile a=c2;
	C* volatile c3=static_cast<C*>(a);

	assertEqual(a->f(), 3, "Complex virtual call 1/5");
	assertEqual(b->f(), 3, "Complex virtual call 2/5");
	assertEqual(c.f(), 3, "Complex virtual call 3/5");

	B b2;
	A* volatile a2=&b2;
	assertEqual(a2->f(), 2, "Complex virtual call 4/5");
	assertEqual(b2.f(), 2, "Complex virtual call 5/5");
}
