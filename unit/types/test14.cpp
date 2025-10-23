// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: Collapsing struct to element 1/3 : SUCCESS
// CHECK: Collapsing struct to element 2/3 : SUCCESS
// CHECK: Collapsing struct to element 3/3 : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2015 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>

struct A
{
	double a[3];
	void f() __attribute__((noinline))
	{
		a[0] = a[1] - a[2];
		a[1] = a[0] + a[1];
		a[2] = a[1] - a[0];
	}
};

void webMain()
{
	// Test collapsing single elements structs to the element itself
	A a;
	a.a[0] = 45;
	a.a[1] = 46;
	a.a[2] = 47;
	a.f();
	assertEqual(a.a[0], -1.0, "Collapsing struct to element 1/3");
	assertEqual(a.a[1], 45.0, "Collapsing struct to element 2/3");
	assertEqual(a.a[2], 46.0, "Collapsing struct to element 3/3");
}
