
//===---------------------------------------------------------------------===//
//	Copyright 2019-2021 Leaning Technlogies
//===----------------------------------------------------------------------===//

// REQUIRES: js

// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// CHECK: Object::operator[] : SUCCESS

#include <cheerp/client.h>
#include <cheerp/clientlib.h>
#include "tests.h"

void webMain()
{
	// Test square parenthesis operator and set_
	client::Object* t = new client::Object();
	t->set_(client::String("a"), new client::String("b"));
	
	client::String* retrieved = (client::String*)(*t)[client::String("a")];
	assertEqual(retrieved, new client::String("b"), "Object::operator[]");
}
