//===---------------------------------------------------------------------===//
//	Copyright 2013 Leaning Technlogies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t

// RUN: regular_only run_if_js compile_mode_js -I%S/../ -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_js compile_mode_js -I%S/../ -o %t/j %s 2>&1 | %FileCheck %s

// CHECK: Downcast 1/2: 2
// CHECK: Downcast 2/2: 2

#include "tests.h"

struct C
{
	float c;
	C():c(3){}
};

struct B
{
	int b;
	B():b(1){}
};

struct A: public B, public C
{
	int a;
	A():a(2){}
};

void webMain()
{
	A a;
	B* volatile b=&a;
	C* volatile c=&a;
	A* volatile a2=static_cast<A*>(b);
	__preexecute_print_case("Downcast 1/2:", a2->a);
	A* volatile a3=static_cast<A*>(c);
	__preexecute_print_case("Downcast 2/2:", a3->a);
}
