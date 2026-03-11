//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: mkdir -p %t

// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1| %FileCheck %s
// RUN: preexec_only run_if_js compile_mode_js -o %t/j %s 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 | %FileCheck %s


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
