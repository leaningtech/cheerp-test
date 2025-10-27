// REQUIRES: js

// RUN: %if_js %cheerp_clang -O1 -frtti -I%S/.. -target cheerp %s -o %t.js %endif
// RUN: %if_js %node %t.js 2>&1 | %FileCheck %s %endif
// CHECK: Operator[] on typed arrays 1/2 : SUCCESS
// CHECK: Operator[] on typed arrays 2/2 : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2016 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>
#include <cheerp/clientlib.h>

void webMain()
{
	client::Int8Array* a = new client::Int8Array(10);
	client::Int8Array& r = *a;
	r[3] = 42;
	assertEqual(r[0], (char)0, "Operator[] on typed arrays 1/2");
	assertEqual(r[3], (char)42, "Operator[] on typed arrays 2/2");
}
