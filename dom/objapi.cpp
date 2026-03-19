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
	client::Object* o = new client::Object();
	o->set_("prop1", new client::Number(1));
	o->set_("prop2", new client::String("var"));
	o->set_("prop3", nullptr);
	client::Array* ar = client::Object::values(o);

	cheerp::console_log("Object::value_length:", ar->get_length());
    // CHECK: Object::value_length: 3
	cheerp::console_log("Object::value of (int)*(*ar)[0]:", (int)*(*ar)[0]);
	// CHECK: Object::value of (int)*(*ar)[0]: 1
	cheerp::console_log("Object::value of (*ar)[1]:", (*ar)[1]->cast<client::String*>());
	// CHECK: Object::value of (*ar)[1]: var		
	cheerp::console_log("Object::value of (*ar)[2]:", (*ar)[2]);
	// CHECK: Object::value of (*ar)[2]: null
}