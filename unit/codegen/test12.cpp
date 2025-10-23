// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// CHECK: High precision strtod : SUCCESS

//===---------------------------------------------------------------------===//
//      Copyright 2014 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>
#include <stdio.h>
#include <stdlib.h>

void webMain()
{
	const char* str="-0.832050323486328125";
	double ret=strtod(str, NULL);
	assertEqual(ret,-0.832050323486328125,"High precision strtod");
}
