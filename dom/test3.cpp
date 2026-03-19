//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: js, regular

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j | %FileCheck %s

#include <tests.h>
#include <string>

void webMain()
{
	//Test string constructor
	client::String* ret=client::String::fromCharCode(0x30);
	assertPrint("String::fromCharCode:", Hex(ret->charCodeAt(0)));
	// CHECK: String::fromCharCode: 0x30

	ret=new client::String("1");
	assertPrint("String from const char *:", Hex(ret->charCodeAt(0)));
	// CHECK: String from const char *: 0x31
}
