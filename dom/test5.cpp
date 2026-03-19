//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: js, regular

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j | %FileCheck %s

#include <cheerp/client.h>
#include <cheerp/clientlib.h>
#include "tests.h"

void webMain()
{
	client::String* a1 = new client::String("Test");
	client::String* a2 = new client::String("Test");
	
	assertEqual(a1==a2, true ,"Comparing client object:");
	// CHECK: Comparing client object: 1
}
