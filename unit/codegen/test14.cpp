// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: Accessing member of stack allocated struct : SUCCESS
// CHECK: Accessing member of heap allocated struct : SUCCESS

//===---------------------------------------------------------------------===//
//      Copyright 2015 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>

struct A
{
	int i;
};

struct B
{
	A* a;
};

void webMain()
{
	volatile B b;
	b.a = new A;
	b.a->i = 42;
	volatile A* a2 = new A[10];
	for(int i=0;i<10;i++)
		a2[i].i = i;
	assertEqual(b.a->i, 42, "Accessing member of stack allocated struct");
	assertEqual((int)a2[8].i, 8, "Accessing member of heap allocated struct");
}
