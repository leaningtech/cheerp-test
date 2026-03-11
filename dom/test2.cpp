//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: js, regular

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j | %FileCheck %s

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
