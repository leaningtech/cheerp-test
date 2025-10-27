// RUN: %if_js %cheerp_clang -O1 -frtti -I%S/.. -target cheerp %s -o %t.js %endif
// RUN: %if_js %node %t.js 2>&1 | %FileCheck %s %endif
// RUN: %if_asmjs %cheerp_clang -O1 -frtti -I%S/.. -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js %endif
// RUN: %if_asmjs %node %t_asmjs.js 2>&1 | %FileCheck %s %endif
// RUN: %if_wasm %cheerp_clang -O1 -frtti -I%S/.. -target cheerp-wasm %s -o %t_wasm.js %endif
// RUN: %if_wasm %node %t_wasm.js 2>&1 | %FileCheck %s %endif
// CHECK: C++ implicit operator=() 1/4 : SUCCESS
// CHECK: C++ implicit operator=() 2/4 : SUCCESS
// CHECK: C++ implicit operator=() 3/4 : SUCCESS
// CHECK: C++ implicit operator=() 4/4 : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2013-2014 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>

struct A
{
	int a;
	float b;
	virtual void f()
	{
	}
	A(int x, float y):a(x),b(y)
	{
	}
};

void webMain()
{
	//Test copy constructor, it must not use memcpy
	A a1(10, 0.2);
	A a2(0,0);

	assertEqual(a2.a, 0, "C++ implicit operator=() 1/4");
	assertEqual(a2.b, 0.f, "C++ implicit operator=() 2/4");
	a2 = a1;
	assertEqual(a2.a, 10, "C++ implicit operator=() 3/4");
	assertEqual(a2.b, 0.2f, "C++ implicit operator=() 4/4");
}
