// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: Virtual Bases Return adjustment 1/1 : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2018 Leaning Technlogies
//===---------------------------------------------------------------------===//

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
	assertEqual(20, ret, "Virtual Bases Return adjustment 1/1");
}
