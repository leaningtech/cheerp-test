
//===---------------------------------------------------------------------===//
//	Copyright 2019-2021 Leaning Technlogies
//===----------------------------------------------------------------------===//

// REQUIRES: js

// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// CHECK: Object to Number : SUCCESS

#include <tests.h>
#include <cheerp/client.h>

void webMain()
{
	//Test Object to Number conversion
	client::Object* o = client::JSON.parse("42.42");
	double dval = (double)*o;
	assertEqual(dval, 42.42, "Object to Number");
}
