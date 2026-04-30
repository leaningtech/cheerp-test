//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>

struct A
{
	double x;
	A():x(f())
	{
		assertPrint("Virtual calls in constructor 1/3:", x);
		// CHECK: Virtual calls in constructor 1/3: 42.0000
	}
	virtual int f()
	{
		return 42;
	}
};

struct B: public A
{
	int b;
	B():b(f())
	{
		assertPrint("Virtual calls in constructor 2/3:", b);
		// CHECK: Virtual calls in constructor 2/3: 43 
	}
	virtual int f()
	{
		return 43;
	}
};

void webMain()
{
	B* b = new B();
	assertPrint("Virtual calls in constructor 3/3:", b->b);
	// CHECK: Virtual calls in constructor 3/3: 43
}
