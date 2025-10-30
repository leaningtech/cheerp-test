//===---------------------------------------------------------------------===//
//	Copyright 2017 Leaning Technlogies
//===----------------------------------------------------------------------===//

// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: PointerAnalyzer safety for covariant returns : SUCCESS

#include <tests.h>

struct A
{
	int val = 42;
	virtual A* covariantFunc()
	{
		return this;
	}
};

struct B: public A
{
	B* covariantFunc() override
	{
		return this;
	}
};

void webMain()
{
	struct A* volatile a = new B;
	A* ret1 = a->covariantFunc();
	// Force REGULAR representation for returned A*
	volatile intptr_t forceRegular = reinterpret_cast<intptr_t>(ret1);
	assertEqual(ret1->val, 42, "PointerAnalyzer safety for covariant returns");
}
