//===---------------------------------------------------------------------===//
//	Copyright 2014 Leaning Technlogies
//===----------------------------------------------------------------------===//

// REQUIRES: js

// RUN: %node %t.js 2>&1 | %FileCheck %s -dump-input=fail
// CHECK: String literal support : SUCCESS

#include <tests.h>

void webMain()
{
	client::String* escaped = new client::String("V\bery evil\t string\n\xff");
	assertEqual(escaped->get_length(), 20, "String literal support");
}
