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

struct J {
	int j{10};
	virtual ~J()
	{
	}
};
struct K {
	int k{20};
	virtual int f() {
		return k;
	}
};
struct B: public K, public  J
{
	int b;
	B():b(1){}
	virtual ~B()
	{
	}
	virtual int f() override
	{
		return b;
	}
};

struct A: public X, public virtual B
{
	int a;
	A():a(2){}
	~A()
	{
	}
	int f() override
	{
		return a;
	}
};

void webMain()
{
	A a;
	A* volatile a2=&a;
	B* volatile b=&a;
	K* volatile k=&a;

	assertPrint("Virtual Bases This adjustment 1/3:", b->f());
	// CHECK: Virtual Bases This adjustment 1/3: 2
	assertPrint("Virtual Bases This adjustment 2/3:", a2->f());
	// CHECK: Virtual Bases This adjustment 2/3: 2
	assertPrint("Virtual Bases This adjustment 3/3:", k->f());
	// CHECK: Virtual Bases This adjustment 3/3: 2
}
