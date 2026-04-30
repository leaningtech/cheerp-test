//===----------------------------------------------------------------------===//
//      Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>

int testSwitch(volatile int16_t* i)
{
	switch((int16_t)*i)
	{
		case (int16_t)10:
			return 0;
		case (int16_t)-1:
			return 1;
		default:
			return 2;
	}
}

void webMain()
{
	volatile int16_t a = 0xffff;
	assertPrint("Switch on 16-bit integers 1/3:", testSwitch(&a)); // CHECK: Switch on 16-bit integers 1/3: 1
	a = -1;
	assertPrint("Switch on 16-bit integers 2/3:", testSwitch(&a)); // CHECK: Switch on 16-bit integers 2/3: 1
	a = 10;
	assertPrint("Switch on 16-bit integers 3/3:", testSwitch(&a)); // CHECK: Switch on 16-bit integers 3/3: 0
}
