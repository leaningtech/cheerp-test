//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>

const char str[] = "testStr";

struct A
{
	const char* ptr;
	float f;
};

A a = { str, -123.5f };

void webMain()
{
	const char* p=a.ptr;
	assertPrint("Access to member pointers to global array:", *p);
	// CHECK: Access to member pointers to global array: t
}
