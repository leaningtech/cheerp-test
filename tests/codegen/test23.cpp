//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s

#include <tests.h>
#include <stdlib.h>

struct A
{
	struct A* a;
};

void webMain()
{
	struct A a;
	a.a = &a;
	assertPrint("Recursive pointer in struct:", (a.a == &a));
	//CHECK: Recursive pointer in struct: 1
}
