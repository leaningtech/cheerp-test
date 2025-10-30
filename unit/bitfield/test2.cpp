//===---------------------------------------------------------------------===//
//	Copyright 2013 Leaning Technlogies
//===----------------------------------------------------------------------===//

// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s

//CHECK: Bitfield test 1/3 : SUCCESS
//CHECK: Bitfield test 2/3 : SUCCESS
//CHECK: Bitfield test 3/3 : SUCCESS


#include <tests.h>

struct A
{
	int a1:15;
	int a2:16;
	int a3:3;
};

void webMain()
{
	A a;
	a.a1=-8191;
	a.a2=8191;
	a.a3=2;
	assertEqual<int>(a.a1, -8191, "Bitfield test 1/3");
	assertEqual<int>(a.a2, 8191, "Bitfield test 2/3");
	assertEqual<int>(a.a3, 2, "Bitfield test 3/3");
}
