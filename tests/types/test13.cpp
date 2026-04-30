//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>

struct A
{
	int i;
};

struct B
{
	int i;
};

void webMain()
{
	volatile B b = { 2 };
	volatile A a = { 1 };
	volatile void * volatile p = &b.i;
	volatile int ai = a.i;
	volatile int bi = b.i;

	assertPrint("Pointers to member variables 1/2:", (int)ai);
	// CHECK: Pointers to member variables 1/2: 1
	assertPrint("Pointers to member variables 2/2:", int(bi));
	// CHECK: Pointers to member variables 2/2: 2
}
