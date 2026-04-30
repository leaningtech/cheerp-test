//===---------------------------------------------------------------------===//
//      Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s

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
