//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s



#include <tests.h>
#include <functional>

int func()
{
	return 42;
}

void webMain()
{
	std::function<int()> f=func;
	assertPrint("Function object:", f());
	// CHECK: Function object: 42
}
