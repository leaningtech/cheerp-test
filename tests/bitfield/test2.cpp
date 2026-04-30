//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s

// CHECK: Bitfield test 1/3: -8191
// CHECK: Bitfield test 2/3: 8191
// CHECK: Bitfield test 2/3: 2


#include "./../tests.h"

struct A
{
	int a1:15;
	int a2:16;
	int a3:3;
};

void webMain()
{
	A a;
	a.a1=-8191;
	a.a2=8191;
	a.a3=2;
	assertPrint("Bitfield test 1/3:", a.a1);
	assertPrint("Bitfield test 2/3:", a.a2);
	assertPrint("Bitfield test 2/3:", a.a3);
}
