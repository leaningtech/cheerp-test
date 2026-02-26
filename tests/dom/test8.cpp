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
	// Test square parenthesis operator and set_
	client::Object* t = new client::Object();
	t->set_(client::String("a"), new client::String("b"));
	
	client::String* retrieved = (client::String*)(*t)[client::String("a")];
	assertPrint("Object::operator[]:", retrieved);
	// CHECK: Object::operator[]: b
}