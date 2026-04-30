//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s



#include <tests.h>

class A
{
public:
	float data[4][4];
};

void webMain()
{
	A a, a2;
	a2.data[1][2] = 42;
	memcpy(&a,&a2,sizeof(A));
	assertPrint("Interaction between arrays of arrays and memory intrinsics:", a.data[1][2]);
	// CHECK: Interaction between arrays of arrays and memory intrinsics: 42.0000
}
