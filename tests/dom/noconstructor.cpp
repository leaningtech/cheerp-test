//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: js, regular

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j | %FileCheck %s

#include <tests.h>
#include <cheerp/client.h>
#include <cheerp/types.h>

namespace client
{
	class FakeClientObj: public Object
	{
	public:
		void set_val(int);
		int get_val();
	};
}

void webMain()
{
	client::FakeClientObj* o = new client::FakeClientObj();
	o->set_val(42);
	assertPrint("Creating client object that do not have a constructor:", o->get_val());
	// assertEqual(o->get_val(), 42, "Creating client object that do not have a constructor:");
	// CHECK: Creating client object that do not have a constructor: 42
	client::TArray<client::String*>* ta = new client::TArray<client::String*>();
	// assertEqual(ta->get_length(), 0, "Creating template array wrapper type");
	assertPrint("Creating template array wrapper type:", ta->get_length());
	// CHECK: Creating template array wrapper type: 0
}