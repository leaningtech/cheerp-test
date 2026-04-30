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

void webMain()
{
	//Test new on classes
	A* a=new A;
	assertPrint("New on class types 1/2:", a->a);
	// CHECK: New on class types 1/2: 42
	A* a2=new (std::nothrow) A;
	assertPrint("Nothrow new on class types 2/2:", a2->a);
	// CHECK: Nothrow new on class types 2/2: 42
}
