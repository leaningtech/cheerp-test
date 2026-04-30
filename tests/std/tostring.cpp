//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// REQUIRES: js, regular
// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s



#include <tests.h>
#include <string>

void webMain()
{
	std::string val = std::to_string(40.0f);
	assertPrint("std::to_string for floats:", (val == "40.000000"));
	// CHECK: std::to_string for floats: 1
}
