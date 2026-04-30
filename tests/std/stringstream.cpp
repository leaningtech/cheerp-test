//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s

#include <tests.h>
#include <sstream>

void webMain()
{
	std::ostringstream str;
	str << "test " << 42;
	assertPrint("ostringstream write:", str.str().c_str());
	// CHECK: ostringstream write: test 42

	std::stringstream str2;
	str2 << "43";
	int res;
	str2 >> res;
	assertPrint("stringstream read int:", res);
	// CHECK: stringstream read int: 43
}
