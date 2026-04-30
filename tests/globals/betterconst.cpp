//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>

struct A
{
	char c;
};

struct B: public A
{
	float f;
};


B b;

A* volatile a1 = &b;

char* volatile c1 = &b.c;

struct C
{
	A a;
	float f;
};

C c = { { 'A' }, 42.0f };

A* volatile a2 = &c.a;

char* volatile c2 = &c.a.c;

float* volatile f2 = &c.f;

void* volatile v = &b;

void webMain()
{
	b.c = 'A';
	b.f = 42.0f;	

	assertPrint("Better generation of constant init for globals 1/6:", a1->c);
	// CHECK: Better generation of constant init for globals 1/6: A
	assertPrint("Better generation of constant init for globals 2/6:", *c1);
	// CHECK: Better generation of constant init for globals 2/6: A
	assertPrint("Better generation of constant init for globals 3/6:", a2->c);
	// CHECK: Better generation of constant init for globals 3/6: A
	assertPrint("Better generation of constant init for globals 4/6:", *c2);
	// CHECK: Better generation of constant init for globals 4/6: A
	assertPrint("Better generation of constant init for globals 5/6:", *f2);
	// CHECK: Better generation of constant init for globals 5/6: 42.0
	assertPrint("Better generation of constant init for globals 6/6:", ((B*)v)->f);
	// CHECK: Better generation of constant init for globals 6/6: 42.0
}






