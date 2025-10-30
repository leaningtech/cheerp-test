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

struct A
{
	int a;
	float b;
	virtual void f()
	{
	}
	A(int x, float y):a(x),b(y)
	{
	}
};

void webMain()
{
	//Test copy constructor, it must not use memcpy
	A a1(10, 0.2);
	A a2(0,0);

	assertPrint("C++ implicit operator=() 1/4:", a2.a);
	// CHECK: C++ implicit operator=() 1/4: 0
	assertPrint("C++ implicit operator=() 2/4:", a2.b);
	// CHECK: C++ implicit operator=() 2/4: 0.000
	a2 = a1;
	assertPrint("C++ implicit operator=() 3/4:", a2.a);
	// CHECK: C++ implicit operator=() 3/4: 10
	assertPrint("C++ implicit operator=() 4/4:", a2.b);
	// CHECK: C++ implicit operator=() 4/4: 0.200
}
