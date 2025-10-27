//===---------------------------------------------------------------------===//
//	Copyright 2014 Leaning Technlogies
//===----------------------------------------------------------------------===//

// REQUIRES: js

// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s

#include <tests.h>
#include <string>

void webMain()
{
	std::string val = std::to_string(40.0f);
	assertEqual<std::string>(val, "40.000000", "std::to_string for floats");
}
