//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 %then_run_js | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1 | %FileCheck %s

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
