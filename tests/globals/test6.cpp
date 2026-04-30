//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>

bool funcCalled = false;

void testMethod()
{
	funcCalled = true;
}

void (*globalMethodPtr)() = NULL;

void webMain()
{
	if(globalMethodPtr==NULL)
		globalMethodPtr = testMethod;
	globalMethodPtr();
	assertPrint<int>("Global function pointer:", funcCalled);
	// CHECK: Global function pointer: 1
}
