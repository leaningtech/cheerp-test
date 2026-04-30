//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>

void webMain()
{
	int a[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int b[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	std::uninitialized_copy(&a[0], &a[7], &b[0]);
	assertPrint("std::uninitialized_copy 1/2:", b[6]);
	// CHECK: std::uninitialized_copy 1/2: 6
	assertPrint("std::uninitialized_copy 2/2:", b[7]); 
	// CHECK: std::uninitialized_copy 2/2: 0
	
	std::uninitialized_fill(&b[0], &b[7], 42);
	assertPrint("std::uninitialized_fill 1/2:", b[6]);
	// CHECK: std::uninitialized_fill 1/2: 42
	assertPrint("std::uninitialized_fill 2/2:", b[7]);
	// CHECK: std::uninitialized_fill 2/2: 0

	std::uninitialized_fill_n(&b[0], 7, 43);
	assertPrint("std::uninitialized_fill_n 1/2:", b[6]);
	// CHECK: std::uninitialized_fill_n 1/2: 43
	assertPrint("std::uninitialized_fill_n 2/2:", b[7]);
	// CHECK: std::uninitialized_fill_n 2/2: 0

}
