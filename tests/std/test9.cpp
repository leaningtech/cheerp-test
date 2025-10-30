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
#include <vector>

void webMain()
{
	std::vector<float> v;
	for(int i=0;i<10;i++)
		v.push_back(i);
	// assertEqual(v.end()-v.begin(), 10L, "std::vector 1/1");
	assertPrint("std::vector 1/2:", v.end()-v.begin());
	// CHECK: std::vector 1/2: 10
	v.erase(v.begin()+3);
	// assertEqual(v.end()-v.begin(), 9L, "std::vector 2/2");
	assertPrint("std::vector 2/2:", v.end()-v.begin());
	// CHECK: std::vector 2/2: 9
}
