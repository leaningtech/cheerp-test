//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>

class A
{
public:
	int a;
	A(int _a):a(_a)
	{
	}
};

void webMain()
{
	//Test new with [[cheerp::nonit]]
	//Note that A has no default constructor
	A* a=new A[2] [[cheerp::noinit]];
	a[1].a = 42;
	assertPrint("New array with [[cheerp::noinit]]:", a[1].a);
	// CHECK: New array with {{\[\[}}cheerp::noinit{{\]\]}}: 42
	A(*a2)[3]=new A[2][3] [[cheerp::noinit]];
	a2[1][1].a = 43;
	assertPrint("New multi array with [[cheerp::noinit]]:", a2[1][1].a);
	// CHECK: New multi array with {{\[\[}}cheerp::noinit{{\]\]}}: 43
	A* a3=new A [[cheerp::noinit]];
	a3->a = 44;
	assertPrint("New with [[cheerp::noinit]]:", a3->a);
	// CHECK: New with {{\[\[}}cheerp::noinit{{\]\]}}: 44
}
