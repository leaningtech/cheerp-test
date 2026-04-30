//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: js, regular
// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


// CHECK: String literal support: 20

#include <tests.h>

void webMain()
{
	client::String* escaped = new client::String("V\bery evil\t string\n\xff");
	assertPrint("String literal support:", escaped->get_length());
}
