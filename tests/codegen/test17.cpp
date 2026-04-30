//===----------------------------------------------------------------------===//
//      Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s

#include <tests.h>

void webMain()
{
	volatile unsigned int a=0x7fffffff;
	a *= 2;
	a += 1;
	volatile int b=0;
	b--;
	unsigned int tmp1=a;
	int tmp2=b;
	assertPrint("Signed arithmetic 1/N:", (tmp1 == tmp2));
	volatile unsigned short a2=0xffff;
	volatile short b2=0;
	b2--;
	unsigned short tmp3=a2;
	short tmp4=b2;
	assertPrint("Signed arithmetic 2/N:", ((signed short)tmp3 == tmp4));
	assertPrint("Signed arithmetic 3/N:", (tmp3 == (unsigned short)tmp4));
}

// CHECK: Signed arithmetic 1/N: 1
// CHECK: Signed arithmetic 2/N: 1
// CHECK: Signed arithmetic 3/N: 1