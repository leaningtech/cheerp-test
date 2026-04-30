//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>

struct A;

struct C
{
	A* a;
};

struct B
{
	C c;
};

struct A
{
	B b;
};

void webMain()
{
	// Test collapsing of recursive struct
	A a;
	a.b.c.a = &a;
	// Do a cast that is only valid if the struct is collapsed
	B* b = reinterpret_cast<B*>(&a);

	assertPrint("Collapsing of recursive structs:", (signed int)(b->c.a == &a));
	// CHECK: Collapsing of recursive structs: 1
}
