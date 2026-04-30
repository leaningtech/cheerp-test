//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>

void webMain()
{
	// Test dynamic stack allocation
	volatile int length = 100;
	double v1[length];
	for(int i=0;i<length;i++)
		v1[i] = double(i)/2;
	assertPrint("Dyn stack 1/2:", v1[1]);
	// CHECK: Dyn stack 1/2: 0.5

	double* v2 = static_cast<double*>(alloca(length*sizeof(double)));
	for(int i=0;i<length;i++)
		v2[i] = double(i)/2;
	assertPrint("Dyn stack 2/2:", v2[1]);
	// CHECK: Dyn stack 2/2: 0.5
}
