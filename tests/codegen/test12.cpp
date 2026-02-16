//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//
// assertEqual compares the values of ret with the full comparison value, however only prints the first 10 digits.
// Therefore the lowered precision in the CHECK line is not an issue. 

// REQUIRES: js, regular

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s

// CHECK: High precision strtod: -0.8320503235

#include <tests.h>
#include <stdio.h>
#include <stdlib.h>

void webMain()
{
	const char* str="-0.832050323486328125";
	double ret=strtod(str, NULL);
	assertEqual(ret, -0.832050323486328125, 0, "High precision strtod:");
}
