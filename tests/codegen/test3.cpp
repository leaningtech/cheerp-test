//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s

#include <tests.h>

void webMain()
{
	const char test[] = {0, 1, 4, 5*2};
	assertPrint("Constant arrays in methods:", (int)test[2]);
	//CHECK: Constant arrays in methods: 4
}
