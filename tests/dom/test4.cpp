//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: js, regular

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j | %FileCheck %s

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
