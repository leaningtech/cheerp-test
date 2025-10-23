// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: Static initialiation of class instances 1/3 : SUCCESS
// CHECK: Static initialiation of class instances 2/3 : SUCCESS
// CHECK: Static initialiation of class instances 3/3 : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2013 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>

class A
{
public:
	int a;
	A():a(42)
	{
		assertEqual(true, true, "Static initialiation of class instances 1/3");
	}
};

int f()
{
	static A a;
	a.a++;
	return a.a;
}

void webMain()
{
	assertEqual(f(), 43, "Static initialiation of class instances 2/3");
	assertEqual(f(), 44, "Static initialiation of class instances 3/3");
}
