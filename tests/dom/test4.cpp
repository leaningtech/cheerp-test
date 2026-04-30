//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: js, regular
// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <cheerp/client.h>
#include <cheerp/clientlib.h>

void webMain()
{
	//Test various variadic methods
	client::console.log("Variadic methods 1/4: SUCCESS");
	// CHECK: Variadic methods 1/4: SUCCESS
	client::console.log(client::String("Variadic methods 2/4: SUCCESS"));
	// CHECK: Variadic methods 2/4: SUCCESS
	client::console.log("Variadic methods 3/4:","SUCCESS");
	// CHECK: Variadic methods 3/4: SUCCESS
	client::console.log(client::String("Variadic methods 4/4:"),client::String("SUCCESS"));
	// CHECK: Variadic methods 4/4: SUCCESS
}
