//===---------------------------------------------------------------------===//
//      Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s

#include <tests.h>

class A
{
public:
	int a;
	A(int i=42):a(i)
	{
	}
};

class B: public A
{
public:
	float f;
	B(int i=43):A(i),f(2.3f)
	{
	}
};

class C
{
public:
	B b;
	C():b(44)
	{
	}
};

class D
{
public:
	B* b;
	D():b(NULL)
	{
		b = new B(45);
	}
};

void webMain()
{
	A a;
	B b;
	C c;
	D d;
	assertPrint("Struct flattening 1/5:", a.a); //CHECK: Struct flattening 1/5: 42
	assertPrint("Struct flattening 2/5:", b.a); //CHECK: Struct flattening 2/5: 43
	assertPrint("Struct flattening 3/5:", b.f); //CHECK: Struct flattening 3/5: 2.2999999523
	assertPrint("Struct flattening 4/5:", c.b.a); //CHECK: Struct flattening 4/5: 44
	assertPrint("Struct flattening 5/5:", d.b->a); //CHECK: Struct flattening 5/5: 45
}
