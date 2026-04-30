//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: regular
// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


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
