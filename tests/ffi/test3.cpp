//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: regular

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1 | %FileCheck %s 

#include <tests.h>
#include <cheerp/client.h>
#include <string>
#include <iostream>

#include <cstddef>

[[cheerp::genericjs]] client::String* s = new client::String("aaa");
std::string ss = "bbb";

int main()
{
	assertPrint("Static constructor 1/2:", (ss == "bbb"));
	// CHECK: Static constructor 1/2: 1
	return 1;
}

[[cheerp::genericjs]] void webMain()
{
	main();
	client::String* i = new client::String("aaa");
	assertPrint("Static constructor 2/2:", i == s);
	// CHECK: Static constructor 2/2: 1
}
