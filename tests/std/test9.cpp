//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


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
