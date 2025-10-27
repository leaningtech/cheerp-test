// RUN: %if_js %cheerp_clang -O1 -frtti -I%S/.. -target cheerp %s -o %t.js %endif
// RUN: %if_js %node %t.js 2>&1 | %FileCheck %s %endif
// RUN: %if_asmjs %cheerp_clang -O1 -frtti -I%S/.. -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js %endif
// RUN: %if_asmjs %node %t_asmjs.js 2>&1 | %FileCheck %s %endif
// RUN: %if_wasm %cheerp_clang -O1 -frtti -I%S/.. -target cheerp-wasm %s -o %t_wasm.js %endif
// RUN: %if_wasm %node %t_wasm.js 2>&1 | %FileCheck %s %endif
// CHECK: New on class types 1/2 : SUCCESS
// CHECK: Nothrow new on class types 2/2 : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2014-2015 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>

class A
{
public:
	int a;
	A():a(42)
	{
	}
};

void webMain()
{
	//Test new on classes
	A* a=new A;
	assertEqual(a->a, 42, "New on class types 1/2");
	A* a2=new (std::nothrow) A;
	assertEqual(a->a, 42, "Nothrow new on class types 2/2");
}
