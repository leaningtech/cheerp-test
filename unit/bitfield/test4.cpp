// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s

//CHECK: Bitfield test 1/2 : SUCCESS
//CHECK: Bitfield test 2/2 : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2013 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>

struct A
{
	short a1:2;
};

struct B: public A
{
	short a2:5;
};

void webMain()
{
	B a;
	a.a1=1;
	a.a2=15;
	assertEqual<int>(a.a1, 1, "Bitfield test 1/2");
	assertEqual<int>(a.a2, 15, "Bitfield test 2/2");
}
