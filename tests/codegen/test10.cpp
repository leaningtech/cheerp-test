//===---------------------------------------------------------------------===//
//      Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s

#include <tests.h>

void webMain()
{
	volatile double d=1;
	const double epsilon = 1.0e-10;

	assertPrint("High precision double literals:", d<1.0+epsilon);

	//CHECK: High precision double literals: 1
}
