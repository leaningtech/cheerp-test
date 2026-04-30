//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: js, regular
// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


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