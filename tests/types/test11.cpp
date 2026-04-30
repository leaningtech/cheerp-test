//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>

struct B;

struct A
{
	A* a;
	A* (*func)(A*);
};

struct B
{
	B* b;
	A* (*func)(A*);
};

A* funcDef(A*)
{
}

A globalA = { NULL, funcDef };

void webMain()
{
	A a;
	a.a = &a;
	volatile B b;
	assertPrint("Recursive structure uses:", (signed int)(a.a == &a));
	// CHECK: Recursive structure uses: 1
}
