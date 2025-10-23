// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: Virtual This adjustment diamond hierarchy 1/1 : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2018 Leaning Technlogies
//===---------------------------------------------------------------------===//

#include <tests.h>

struct X {
	int x{10};
	virtual int foo() {
		return x;
	};
};
struct Y {
	int y{20};
	virtual int bar() {
		return y;
	};
};
struct A: public Y, public X {
	int a{1};
	virtual int foo() {
		return a;
	}
};
struct B: virtual A {
	int b{2};
	virtual int foo() {
		return b;
	}
};
struct C: virtual B {
	int c{3};
	virtual int foo() {
		return c;
	}
};
struct D: virtual C {
	int d{4};
};


int foo(X* x) {
	return x->foo();
}
void webMain()
{
	D d;
	X* x = &d;
	int ret = foo(x);
	assertEqual(3, ret, "Virtual This adjustment diamond hierarchy 1/1");
}
