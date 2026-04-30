//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>

struct X
{
	double x;
	virtual ~X()
	{
	}
};

struct B
{
	int b;
	B():b(1){}
	virtual ~B()
	{
	}
	virtual int f()
	{
		return b;
	}
};

struct A: public X, public B
{
	int a;
	A():a(2){}
	~A()
	{
	}
	int f()
	{
		return a;
	}
};

void webMain()
{
	A a;
	A* volatile a2=&a;
	B* volatile b=&a;

	assertPrint("Simple virtual call 1/2:", b->f());
	// CHECK: Simple virtual call 1/2: 2
	assertPrint("Simple virtual call 2/2:", a2->f());
	// CHECK: Simple virtual call 2/2: 2
}
