//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: js, regular
// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>
#include <cheerp/client.h>

void webMain()
{
	//Test Object to Number conversion
	client::Object* o = client::JSON.parse("42.42");
	double dval = (double)*o;
	assertPrint("Object to Number:", dval);
	//CHECK: Object to Number: 42.42
}
