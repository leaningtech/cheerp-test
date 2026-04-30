//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s

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
