//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// REQUIRES: regular
// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s

#include <iostream>

void webMain()
{
	std::cout << "Cout output" << std::endl;
	std::cerr << "Cerr output" << std::endl;
	// CHECK: Cout output
	// CHECK: Cerr output
}
