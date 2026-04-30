//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s

// CHECK: Bitfield test 1/3: 7
// CHECK: Bitfield test 2/3: 1

#include "./../tests.h"

struct A
{
	short a1:15;
	short a2:2;
};

void webMain()
{
	A a;
	a.a1=7;
	a.a2=1;

	assertPrint("Bitfield test 1/3:", a.a1);
	assertPrint("Bitfield test 2/3:", a.a2);
}
