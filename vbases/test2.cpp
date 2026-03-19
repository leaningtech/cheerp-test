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
	assertPrint("Virtual This adjustment diamond hierarchy:", ret);
	// CHECK: Virtual This adjustment diamond hierarchy: 3
}
