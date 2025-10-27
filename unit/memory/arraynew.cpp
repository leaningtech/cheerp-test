// RUN: %if_js %cheerp_clang -O1 -frtti -I%S/.. -target cheerp %s -o %t.js %endif
// RUN: %if_js %node %t.js 2>&1 | %FileCheck %s %endif
// RUN: %if_asmjs %cheerp_clang -O1 -frtti -I%S/.. -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js %endif
// RUN: %if_asmjs %node %t_asmjs.js 2>&1 | %FileCheck %s %endif
// RUN: %if_wasm %cheerp_clang -O1 -frtti -I%S/.. -target cheerp-wasm %s -o %t_wasm.js %endif
// RUN: %if_wasm %node %t_wasm.js 2>&1 | %FileCheck %s %endif
// CHECK: Array new support 1/2 : SUCCESS
// CHECK: Array new support 2/2 : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2018 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>

class A
{
public:
	static int constructionCount;
	static int destructionCount;
	int a;
	A():a(42)
	{
		constructionCount++;
	}
	~A()
	{
		destructionCount++;
	}
};

int A::constructionCount = 0;
int A::destructionCount = 0;

void webMain()
{
	A* a = new A[10];
	delete[] a;
	assertEqual(A::constructionCount, 10, "Array new support 1/2");
	assertEqual(A::destructionCount, 10, "Array new support 2/2");

}
