// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: Signed arithmetic 1 : SUCCESS

//===---------------------------------------------------------------------===//
//      Copyright 2015 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>

void webMain()
{
	volatile unsigned int a=0x7fffffff;
	a *= 2;
	a += 1;
	volatile int b=0;
	b--;
	unsigned int tmp1=a;
	int tmp2=b;
	assertEqual(tmp1 == tmp2, true,"Signed arithmetic 1");
	volatile unsigned short a2=0xffff;
	volatile short b2=0;
	b2--;
	unsigned short tmp3=a2;
	short tmp4=b2;
	assertEqual<short>(tmp3,tmp4,"Signed arithmetic 2");
	assertEqual<unsigned short>(tmp3,tmp4,"Signed arithmetic 3");
}
