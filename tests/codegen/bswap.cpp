//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>

uint16_t a = 0x11AA;

void webMain()
{
	//Test that bswaps are properly handled
	volatile uint16_t b = __builtin_bswap16(a);

        uint16_t b2 = b;

	assertPrint("bswap support 1/2:", Hex(a));
	// CHECK: bswap support 1/2: 0x11aa
	assertPrint("bswap support 2/2:", Hex(b2));
	// CHECK: bswap support 2/2: 0xaa11
}
