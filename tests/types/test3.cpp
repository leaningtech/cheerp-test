//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s

#include <tests.h>

class A
{
};

class B: public A
{
};

class C: public B
{
};

void webMain()
{
	C c;
	B* volatile b = &c;
	A* volatile a = &c;
	assertPrint("ran without issue");
	// CHECK: ran without issue 
}
