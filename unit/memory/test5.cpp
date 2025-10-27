// RUN: %if_js %cheerp_clang -O1 -frtti -I%S/.. -target cheerp %s -o %t.js %endif
// RUN: %if_js %node %t.js 2>&1 | %FileCheck %s %endif
// RUN: %if_asmjs %cheerp_clang -O1 -frtti -I%S/.. -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js %endif
// RUN: %if_asmjs %node %t_asmjs.js 2>&1 | %FileCheck %s %endif
// RUN: %if_wasm %cheerp_clang -O1 -frtti -I%S/.. -target cheerp-wasm %s -o %t_wasm.js %endif
// RUN: %if_wasm %node %t_wasm.js 2>&1 | %FileCheck %s %endif
// CHECK: Pointer arithmetic on pointer to member : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2013 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>

struct A
{
	int a;
};

void webMain()
{
	//Test copy constructor, it must not use memcpy
	A a1;
	a1.a = 42;

	volatile int count = 1;
	int* base = &a1.a;
	for(int i=0;i<count;i++)
		base[i]++;

	assertEqual(a1.a, 43, "Pointer arithmetic on pointer to member");
}
