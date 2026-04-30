//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s




#include <tests.h>

struct A
{
	int a;
	virtual int f()
	{
		return 1;
	}
	virtual ~A()
	{
	}
};

struct B: public A
{
	virtual int f()
	{
		return 2;
	}
	virtual ~B()
	{
	}
};

struct C: public B
{
	virtual int f()
	{
		return 3;
	}
	virtual ~C()
	{
	}
};

void webMain()
{
	C c;
	C* volatile c2=&c;
	B* volatile b=c2;
	A* volatile a=c2;
	C* volatile c3=static_cast<C*>(a);

	assertPrint("Complex virtual call 1/5:", a->f());
	// CHECK: Complex virtual call 1/5: 3
	assertPrint("Complex virtual call 2/5:", b->f());
	// CHECK: Complex virtual call 2/5: 3
	assertPrint("Complex virtual call 3/5:", c.f());
	// CHECK: Complex virtual call 3/5: 3

	B b2;
	A* volatile a2=&b2;
	assertPrint("Complex virtual call 4/5:", a2->f());
	// CHECK: Complex virtual call 4/5: 2
	assertPrint("Complex virtual call 5/5:", b2.f());
	// CHECK: Complex virtual call 5/5: 2
}
