//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s

// CHECK: 64-bit wide pointer offsets 1/3: f
// CHECK: 64-bit wide pointer offsets 2/3: h
// CHECK: 64-bit wide pointer offsets 3/3: d

#include <tests.h>

void webMain()
{
	const char* buf = "abcdefghi";
	const char* volatile a = buf+5;
	volatile long long off = 2;
	const char* volatile b = a + off;
	const char* volatile c = a - off;

	assertPrint("64-bit wide pointer offsets 1/3:", *a);
	assertPrint("64-bit wide pointer offsets 2/3:", *b);
	assertPrint("64-bit wide pointer offsets 3/3:", *c);
}
