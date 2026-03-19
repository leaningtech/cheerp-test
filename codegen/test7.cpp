//===---------------------------------------------------------------------===//
//      Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_js compile_mode_js -o %t/j %s 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 | %FileCheck %s

#include <tests.h>
#include <array>
#include <cstdio>

std::array<int,2> arr({{1, 2}});

void webMain()
{
	int index = 0;
	for (const auto & elem: arr)
	{
		if(index==0)
			assertPrint("Range for on std::array 1/3:", elem); // CHECK: Range for on std::array 1/3: 1
		else if(index==1)
			assertPrint("Range for on std::array 2/3:", elem); // CHECK: Range for on std::array 2/3: 2
		index++;
	}
	assertPrint("Range for on std::array 3/3:", index); // CHECK: Range for on std::array 3/3: 2
}
