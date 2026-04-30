//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>
#include <new>

void webMain()
{
	//Test placement new
	int a;
	new (&a) int(42);
	assertPrint("Placement new:", a);
	// CHECK: Placement new: 42
	//This should cause an error
	//new ((void*)&a) int(42);
}
