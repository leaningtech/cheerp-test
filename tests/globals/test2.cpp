//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>

struct A
{
	int x,y;
	A():x(1),y(2) {}
};

static A a;

static int* ptrX=&a.x;

void webMain()
{
	int a=*ptrX;
	assertPrint("Access to member of global structures:", a);
	// CHECK: Access to member of global structures: 1
}
