//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>

class A {
public:
	virtual ~A() { }
};

class B : virtual public A {
};

class C : public B, virtual public A {
public:
	virtual int foo();
};

int C::foo()
{
	return 1;
}
void webMain()
{
	C c;
	int ret = c.foo();
	assertPrint("Nearly empty virtual bases 1/1:", ret);
	// CHECK: Nearly empty virtual bases 1/1: 1
}
