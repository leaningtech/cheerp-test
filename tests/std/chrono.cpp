//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// REQUIRES: regular
// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s



#include <tests.h>
#include <chrono>

void webMain()
{
	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
	std::chrono::high_resolution_clock::time_point t2 = t1+std::chrono::high_resolution_clock::duration(1);
	assertPrint("std::chrono test:", (t2-t1).count());
	// CHECK: std::chrono test: 1
}
