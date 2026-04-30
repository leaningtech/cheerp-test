//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: regular
// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s

// CHECK: set_/get_ in namespace 1/2: 36
// CHECK: set_/get_ in namespace 2/2: -36
// CHECK: get_ * in namespace 1/2: -36
// CHECK: get_ * in namespace 2/2: 100

#include <cheerp/client.h>
#include "tests.h"

namespace [[cheerp::genericjs]] client
{
	namespace [[cheerp::genericjs]] someNamespace
	{
		void set_(int a, int b);
		int get_(int a);
		void set_(const String& a, int b);
		int get_(const String& a);

		void set_someString(int b);
		int get_someString();	
	}
}

namespace someNamespace
{
	//Needed to force the namespace to be declared
	void [[cheerp::jsexport]] emptyFunc()
	{
	}
}

void [[cheerp::genericjs]] webMain()
{
	// Test set_ and get_ generic versions
	client::someNamespace::set_(6, 36);
	client::someNamespace::set_(client::String("someString"), -36);
	assertPrint("set_/get_ in namespace 1/2:", client::someNamespace::get_(6));
	assertPrint("set_/get_ in namespace 2/2:", client::someNamespace::get_(client::String("someString")));

	// Test set_ and get_ non-generic versions
	assertPrint("get_ * in namespace 1/2:", client::someNamespace::get_someString());
	client::someNamespace::set_someString(100);
	assertPrint("get_ * in namespace 2/2:", client::someNamespace::get_someString());
}
