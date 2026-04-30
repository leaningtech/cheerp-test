//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>

struct A
{
	int a;
};

void webMain()
{
	//Test copy constructor, it must not use memcpy
	A a1;
	a1.a = 42;

	volatile int count = 1;
	int* base = &a1.a;
	for(int i=0;i<count;i++)
		base[i]++;

	assertPrint("Pointer arithmetic on pointer to member:", a1.a);
	// CHECK: Pointer arithmetic on pointer to member: 43
}
