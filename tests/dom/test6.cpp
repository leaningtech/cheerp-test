//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: js, regular
// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <cheerp/client.h>
#include <cheerp/clientlib.h>
#include "tests.h"

client::String* volatile globalString;

void webMain()
{
	client::String* a1 = new client::String("Test");
	globalString = a1;
	
	assertEqual(globalString==a1, true, "Comparing client objects from globals:");
	// CHECK: Comparing client objects from globals: 1
}
