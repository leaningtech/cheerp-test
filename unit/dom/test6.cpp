//===---------------------------------------------------------------------===//
//	Copyright 2019-2021 Leaning Technlogies
//===----------------------------------------------------------------------===//

// REQUIRES: js

// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// CHECK: Comparing client objects from globals : SUCCESS

#include <cheerp/client.h>
#include <cheerp/clientlib.h>
#include "tests.h"

client::String* volatile globalString;

void webMain()
{
	client::String* a1 = new client::String("Test");
	globalString = a1;
	
	assertEqual(globalString==a1, true, "Comparing client objects from globals");
}
