//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>

struct BaseStruct
{
	int a;
	float b;
};

struct DerivedStruct
{
	BaseStruct base;
	int c;
};

struct DerivedStruct2
{
	DerivedStruct2():base({2,4}),c(5)
	{
	}
	BaseStruct base;
	int c;
};

DerivedStruct2 gd2;

void webMain()
{
	//Test merging of the first struct member into the class
	//this is useful to support C-style polymorphism
	DerivedStruct s = { { 1, 2.5f }, 2 };
	DerivedStruct* volatile d = &s;

	assertPrint("C-style polymorphism 1/5:", d->base.a);
	// CHECK: C-style polymorphism 1/5: 1
	assertPrint("C-style polymorphism 2/5:", d->base.b);
	// CHECK: C-style polymorphism 2/5: 2.5
	assertPrint("C-style polymorphism 3/5:", d->c);
	// CHECK: C-style polymorphism 3/5: 2

	BaseStruct* volatile b = (BaseStruct*)d;
	assertPrint("C-style polymorphism 4/5:", b->a);
	// CHECKL C-style polymorphism 4/5: 1
	assertPrint("C-style polymorphism 5/5:", b->b);
	// CHECK: C-style polymorphism 5/5: 2.5
}
