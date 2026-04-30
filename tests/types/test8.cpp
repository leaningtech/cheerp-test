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
	}
};

class B
{
public:
	A a[2];
};

void webMain()
{
	B b;
	assertPrint("Initialization of arrays in classes:", b.a[1].a);
	// CHECK: Initialization of arrays in classes: 42
};
