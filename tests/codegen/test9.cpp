//===---------------------------------------------------------------------===//
//      Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s

#include <tests.h>

void __attribute__((noinline)) indirect_mul2(int* r, int* a)
{
	*r = 0;
	*r = (*a+*r) * 2;
}

void webMain()
{
	int a = 2;
	int r;

	indirect_mul2(&r, &a);
	assertPrint("Indirect multiply:", (float)r); //CHECK: Indirect multiply: 4.0 
}
