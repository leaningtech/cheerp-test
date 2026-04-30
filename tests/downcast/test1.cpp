//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


// CHECK: Downcast 1/2: 2
// CHECK: Downcast 2/2: 2

#include "../tests.h"

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
	assertPrint("Downcast 1/2:", a2->a);
	A* volatile a3=static_cast<A*>(c);
	assertPrint("Downcast 2/2:", a3->a);
}
