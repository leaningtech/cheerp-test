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
	A():a(1)
	{
	}
	virtual A* clone()
	{
		return new A(*this);
	}
	virtual ~A()
	{
	}
};

struct B: public A
{
	B()
	{
		a=2;
	}
	virtual ~B()
	{
	}
	virtual B* clone()
	{
		return new B(*this);
	}
};

struct C
{
	virtual ~C()
	{
	}
	int c;
};

struct D: public C, public A
{
	D()
	{
		a=3;
	}
	virtual ~D()
	{
	}
	virtual D* clone()
	{
		return new D(*this);
	}
};

void webMain()
{
	B b;
	A* volatile a=&b;
	A* a2=a->clone();
	B* b2=b.clone();

	assertPrint("Covariant return types 1/4:", a2->a);
	// CHECK: Covariant return types 1/4: 2
	assertPrint("Covariant return types 2/4:", b2->a);
	// CHECK: Covariant return types 2/4: 2

	D d;
	A* volatile a3=&d;
	A* a4=a3->clone();
	D* d2=d.clone();

	assertPrint("Covariant return types 3/4:", a4->a);
	// CHECK: Covariant return types 3/4: 3
	assertPrint("Covariant return types 4/4:", d2->a);
	// CHECK: Covariant return types 4/4: 3
}
