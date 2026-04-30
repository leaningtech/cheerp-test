//===---------------------------------------------------------------------===//
//      Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s

#include <tests.h>
#include <math.h>

// Test alloca merging on libc pow. It is implemented using tons of unions.

void webMain()
{
	volatile float val = 2.0f;
	float ret = powf(val, 10.0f);
	assertPrint("libc pow:", ret);
	//CHECK: libc pow: 1024.0000000000
}
