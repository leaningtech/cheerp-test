// RUN: %if_js %cheerp_clang -O1 -frtti -I%S/.. -target cheerp %s -o %t.js %endif
// RUN: %if_js %node %t.js 2>&1 | %FileCheck %s %endif
// RUN: %if_asmjs %cheerp_clang -O1 -frtti -I%S/.. -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js %endif
// RUN: %if_asmjs %node %t_asmjs.js 2>&1 | %FileCheck %s %endif
// RUN: %if_wasm %cheerp_clang -O1 -frtti -I%S/.. -target cheerp-wasm %s -o %t_wasm.js %endif
// RUN: %if_wasm %node %t_wasm.js 2>&1 | %FileCheck %s %endif
// CHECK: New array with {{\[\[}}cheerp::noinit{{\]\]}} : SUCCESS
// CHECK: New multi array with {{\[\[}}cheerp::noinit{{\]\]}} : SUCCESS
// CHECK: New with {{\[\[}}cheerp::noinit{{\]\]}} : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2014 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>

class A
{
public:
	int a;
	A(int _a):a(_a)
	{
	}
};

void webMain()
{
	//Test new with [[cheerp::nonit]]
	//Note that A has no default constructor
	A* a=new A[2] [[cheerp::noinit]];
	a[1].a = 42;
	assertEqual(a[1].a, 42, "New array with [[cheerp::noinit]]");
	A(*a2)[3]=new A[2][3] [[cheerp::noinit]];
	a2[1][1].a = 43;
	assertEqual(a2[1][1].a, 43, "New multi array with [[cheerp::noinit]]");
	A* a3=new A [[cheerp::noinit]];
	a3->a = 44;
	assertEqual(a3->a, 44, "New with [[cheerp::noinit]]");
}
