//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>

struct A
{
	double a[3];
	void f() __attribute__((noinline))
	{
		a[0] = a[1] - a[2];
		a[1] = a[0] + a[1];
		a[2] = a[1] - a[0];
	}
};

void webMain()
{
	// Test collapsing single elements structs to the element itself
	A a;
	a.a[0] = 45;
	a.a[1] = 46;
	a.a[2] = 47;
	a.f();
	// assertEqual(a.a[0], -1.0, "Collapsing struct to element 1/3");
	assertPrint("Collapsing struct to element 1/3:", a.a[0]);
	// CHECK: Collapsing struct to element 1/3: -1.0000000000
	// assertEqual(a.a[1], 45.0, "Collapsing struct to element 2/3");
	assertPrint("Collapsing struct to element 2/3:", a.a[1]); 
	// CHECK: Collapsing struct to element 2/3: 45.0000000000
	// assertEqual(a.a[2], 46.0, "Collapsing struct to element 3/3");
	assertPrint("Collapsing struct to element 3/3:", a.a[2]);
	// CHECK: Collapsing struct to element 3/3: 46.0000000000
}
