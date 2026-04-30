//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//
// assertEqual compares the values of ret with the full comparison value, however only prints the first 10 digits.
// Therefore the lowered precision in the CHECK line is not an issue. 

// REQUIRES: js, regular
// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


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
