//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_js compile_mode_js -o %t/j %s 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 | %FileCheck %s

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
	assertPrint("PointerAnalyzer safety for covariant returns:", ret1->val);
	// CHECK: PointerAnalyzer safety for covariant returns: 42
}
