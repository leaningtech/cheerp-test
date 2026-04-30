//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s

// CHECK: Bitfield test 1/3: 1
// CHECK: Bitfield test 2/3: 15

#include "./../tests.h"	

struct A
{
	short a1:2;
};

struct B: public A
{
	short a2:5;
};

void webMain()
{
	B a;
	a.a1=1;
	a.a2=15;

	assertPrint("Bitfield test 1/3:", a.a1);
	assertPrint("Bitfield test 2/3:", a.a2);
}
