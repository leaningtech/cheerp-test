//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s

// CHECK: PHI support 1/2: 2
// CHECK: PHI support 2/2: 1

#include <tests.h>

void webMain()
{
	//Test recursive dependency in PHIs
	int a = 1;
	int b = 2;

	for(volatile int i=0;i<3;i++)
	{
		int c=a;
		a=b;
		b=c;
	}

	assertPrint("PHI support 1/2:", a);
	assertPrint("PHI support 2/2:", b);
}
