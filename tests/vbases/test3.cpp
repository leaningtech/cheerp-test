//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s



#include <tests.h>

struct J {
	int j{100};
	virtual ~J(){}
};
struct K {
	int k{200};
	virtual int bar() {
		return k;
	}
};

struct X: public J, public virtual K {
	int x{10};
	virtual int bar() {
		return x;
	}
};

struct Y: public  X {
	int y{20};
	virtual int bar() {
		return y;
	}
};


struct A {
	int a{1};
	virtual K* foo() {
		return new X();
	}
};

struct B: public virtual A {
	int b{2};
	virtual Y* foo() override {
		return new Y();
	}
};

int foo(A* a) {
	K* k =  a->foo();
	return k->bar();
}

void webMain()
{
	B b;
	int ret = foo(&b);
	assertPrint("Virtual Bases Return adjustment:", ret);
	// CHECK: Virtual Bases Return adjustment: 20
}
