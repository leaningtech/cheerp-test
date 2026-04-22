//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: mkdir -p %t

// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1| %FileCheck %s
// RUN: preexec_only run_if_js compile_mode_js -o %t/j %s 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 | %FileCheck %s

#include <tests.h>
#include <stdio.h>

void webMain()
{
	std::string test = "padding=1,2,3,4";
	int a,b,c,d;
	sscanf(test.c_str(), "padding=%d,%d,%d,%d", &a, &b, &c, &d);
	assertPrint("sscanf 1/4:", a);
	// CHECK: sscanf 1/4: 1
	assertPrint("sscanf 2/4:", b);
	// CHECK: sscanf 2/4: 2
	assertPrint("sscanf 3/4:", c);
	// CHECK: sscanf 3/4: 3
	assertPrint("sscanf 4/4:", d);
	// CHECK: sscanf 4/4: 4
}
