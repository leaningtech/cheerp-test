//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>

struct A
{
	int a;
	float b;
	virtual void f()
	{
	}
	A(int x, float y):a(x),b(y)
	{
	}
};

void webMain()
{
	//Test copy constructor, it must not use memcpy
	A a1(10, 0.2);
	A a2(0,0);

	assertPrint("C++ implicit operator=() 1/4:", a2.a);
	// CHECK: C++ implicit operator=() 1/4: 0
	assertPrint("C++ implicit operator=() 2/4:", a2.b);
	// CHECK: C++ implicit operator=() 2/4: 0.000
	a2 = a1;
	assertPrint("C++ implicit operator=() 3/4:", a2.a);
	// CHECK: C++ implicit operator=() 3/4: 10
	assertPrint("C++ implicit operator=() 4/4:", a2.b);
	// CHECK: C++ implicit operator=() 4/4: 0.200
}
