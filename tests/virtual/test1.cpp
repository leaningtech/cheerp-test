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

struct X
{
	double x;
	virtual ~X()
	{
	}
};

struct B
{
	int b;
	B():b(1){}
	virtual ~B()
	{
	}
	virtual int f()
	{
		return b;
	}
};

struct A: public X, public B
{
	int a;
	A():a(2){}
	~A()
	{
	}
	int f()
	{
		return a;
	}
};

void webMain()
{
	A a;
	A* volatile a2=&a;
	B* volatile b=&a;

	assertPrint("Simple virtual call 1/2:", b->f());
	// CHECK: Simple virtual call 1/2: 2
	assertPrint("Simple virtual call 2/2:", a2->f());
	// CHECK: Simple virtual call 2/2: 2
}
