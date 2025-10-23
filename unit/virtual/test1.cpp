// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: Simple virtual call 1/2 : SUCCESS
// CHECK: Simple virtual call 2/2 : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2013 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>

struct X
{
	double x;
	virtual ~X()
	{
	}
};

struct B
{
	int b;
	B():b(1){}
	virtual ~B()
	{
	}
	virtual int f()
	{
		return b;
	}
};

struct A: public X, public B
{
	int a;
	A():a(2){}
	~A()
	{
	}
	int f()
	{
		return a;
	}
};

void webMain()
{
	A a;
	A* volatile a2=&a;
	B* volatile b=&a;

	assertEqual(b->f(), 2, "Simple virtual call 1/2");
	assertEqual(a2->f(),2, "Simple virtual call 2/2");
}
