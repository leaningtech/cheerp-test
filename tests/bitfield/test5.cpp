//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s

// CHECK: Bitfield test 1/2: 23
// CHECK: Bitfield test 2/2: 0xdeadbeef

#include "./../tests.h"

struct A
{
	long long a1:63;
	int b;
};

void webMain()
{
	A a;
	a.a1=(1LL<<61)|23;
	a.b=0xdeadbeef;

	// Bitfield stores lower 63 bits. Test checks we can extract just the low bits.
	assertPrint("Bitfield test 1/2:", (int)(a.a1 & 0x3F));  // Extract lowest 6 bits (23)
	assertPrint("Bitfield test 2/2:", Hex((unsigned int)a.b));
}
