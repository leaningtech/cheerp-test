//===----------------------------------------------------------------------===//
//      Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s

#include <tests.h>
#include <math.h>

void webMain()
{
	volatile float a = -42.0f;
	volatile float b = fabsf(a);
	assertPrint("fabs:", (float)b);
}

// CHECK: fabs: 42.0000000000