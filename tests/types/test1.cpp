//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>

void webMain()
{
	//Test objects of type float
	float a = 0.2f;

	// assertEqual(a, 0.2f, "Float, initialization");
	assertPrint("Float, initialization:", a);
	// CHECK: Float, initialization: 0.20000000
}
