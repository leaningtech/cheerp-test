//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: mkdir -p %t

// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s


#include <tests.h>
#include <string>

void webMain()
{
	std::string val = std::to_string(40.0f);
	assertPrint("std::to_string for floats:", (val == "40.000000"));
	// CHECK: std::to_string for floats: 1
}
