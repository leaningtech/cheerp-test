//===----------------------------------------------------------------------===//
//      Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>

struct A
{
	int i;
};

struct B
{
	A* a;
};

void webMain()
{
	volatile B b;
	b.a = new A;
	b.a->i = 42;
	volatile A* a2 = new A[10];
	for(int i=0;i<10;i++)
		a2[i].i = i;
	assertPrint("Accessing member of stack allocated struct:", b.a->i);
	// CHECK: Accessing member of stack allocated struct: 42

	assertPrint("Accessing member of heap allocated struct:", (int)a2[8].i);
	// CHECK: Accessing member of heap allocated struct: 8
	
}
