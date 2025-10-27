
//===---------------------------------------------------------------------===//
//	Copyright 2019-2021 Leaning Technlogies
//===----------------------------------------------------------------------===//

// REQUIRES: js

// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// CHECK: String::fromCharCode : SUCCESS
// CHECK: String from const char* : SUCCESS

#include <tests.h>
#include <string>

void webMain()
{
	//Test string constructor
	client::String* ret=client::String::fromCharCode(0x30);
	assertEqual(ret->charCodeAt(0), 0x30, "String::fromCharCode");

	ret=new client::String("1");
	assertEqual(ret->charCodeAt(0), 0x31, "String from const char*");
}
