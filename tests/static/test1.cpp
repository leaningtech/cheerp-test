//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>

class A
{
public:
	int a;
	A():a(42)
	{
		assertPrint("Static initialisation of class instances 1/3:", a);
		// assertEqual(true, true, "Static initialiation of class instances  1/3");
	}
};

int f()
{
	static A a;
	a.a++;
	return a.a;
}

void webMain()
{	
	assertPrint("Static initialisation of class instances 2/3:", f());
	// CHECK: Static initialisation of class instances 2/3: 43
	assertPrint("Static initialisation of class instances 3/3:", f());
	// CHECK: Static initialisation of class instances 3/3: 44
}
