//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>

class A
{
public:
	int a;
};

class B: public A
{
public:
	int b;
};

class C: public B
{
public:
	int c;
};

void webMain()
{
	C c;
	B* b = &c;
	A* a = &c;
	B* b2 = static_cast<B*>(a);
	assertPrint("Pointer equality for classes with downcast array:", (signed int)(b == b2));
	// CHECK: Pointer equality for classes with downcast array: 1
}
