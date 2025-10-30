
//===---------------------------------------------------------------------===//
//	Copyright 2019-2021 Leaning Technlogies
//===----------------------------------------------------------------------===//

// REQUIRES: js

// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s

//CHECK: Variadic methods 1/4 : SUCCESS
//CHECK: Variadic methods 2/4 : SUCCESS
//CHECK: Variadic methods 3/4 : SUCCESS
//CHECK: Variadic methods 4/4 : SUCCESS

#include <cheerp/client.h>
#include <cheerp/clientlib.h>

void webMain()
{
	//Test various variadic methods
	client::console.log("Variadic methods 1/4 : SUCCESS");
	client::console.log(client::String("Variadic methods 2/4 : SUCCESS"));
	client::console.log("Variadic methods 3/4 :","SUCCESS");
	client::console.log(client::String("Variadic methods 4/4 :"),client::String("SUCCESS"));
}

