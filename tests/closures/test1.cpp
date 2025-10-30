//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: js, regular

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 %then_run_js | %FileCheck %s

#include <cheerp/client.h>
#include "./../tests.h"

// Test cheerpCreateClosure and cheerpCreateClosureSplit, with multiple arguments
// and return values

void webMain()
{
	int c = unitBlackBox(3);
	int d = unitBlackBox(4);
	auto cb = cheerp::Callback([c,d](int a, int b)
	{
		return a+b+c+d;
	});
	auto cbsplit = cheerp::Callback([c](int a, int b)
	{
		return a+b+c;
	});
	assertPrint("Basic closure creation 1/2:", reinterpret_cast<int(*)(int, int)>(cb)(1,2));
	assertPrint("Basic closure creation 2/2:", reinterpret_cast<int(*)(int, int)>(cbsplit)(1,2));
	// CHECK: Basic closure creation 1/2: 10
	// CHECK: Basic closure creation 2/2: 6
}
