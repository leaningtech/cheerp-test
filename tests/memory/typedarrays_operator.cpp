//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// REQUIRES: js, regular
// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s



#include <tests.h>
#include <cheerp/clientlib.h>

void webMain()
{
	client::Int8Array* a = new client::Int8Array(10);
	client::Int8Array& r = *a;
	r[3] = 42;
	assertPrint("Operator[] on typed arrays 1/2:", (int)r[0]);
	// CHECK: Operator[] on typed arrays 1/2: 0
	assertPrint("Operator[] on typed arrays 2/2:", (int)r[3]);
	// CHECK: Operator[] on typed arrays 2/2: 42
}
