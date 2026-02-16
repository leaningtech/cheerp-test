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

static int A_foo_called = 0;
static int A_bar_called = 0;
static int B_foo_called = 0;
static int B_bar_called = 0;
static int C_foo_called = 0;
static int C_bar_called = 0;
static int D_foo_called = 0;
static int D_bar_called = 0;

class I {
public:
	virtual void foo() = 0;// pure virtual function
	virtual void bar() = 0;// pure virtual function
};

class A : public I {
public:
	virtual void foo() {
		A_foo_called++;
	};

	virtual void bar() {
		A_bar_called++;
	}
};

class B : public A {
public:
	virtual void foo() {
		B_foo_called++;
	};

	virtual void bar() {
		B_bar_called++;
	}
};

class C : public I {
public:
	virtual void foo() {
		C_foo_called++;
	};

	virtual void bar() {
		C_bar_called++;
	}
};

class D : public A, public C {
public:
	virtual void foo() {
		D_foo_called++;
	};

	virtual void bar() {
		D_bar_called++;
	}
};

class E {
public:
	virtual int getData()
	{
		return 0;
	}
};

class F: public A, public E
{
public:
	F():i1(42),f2(43)
	{
	}
	int getData() override
	{
		return i1;
	}
	int i1;
	float f2;
};

void testTypeidName() {
	{
		I* a = new A();
		I* t = dynamic_cast<A*>(a);
		const char *name = typeid(t).name();
		assertPrint("typeid().name() support 1/4:", name);
		// CHECK: typeid().name() support 1/4: P1I
	}

	{
		int i = 0;
		const char *name = typeid(i).name();
		assertPrint("typeid().name() support 2/4:", name);
		// CHECK: typeid().name() support 2/4: i
	}

	{
		double *doubleptr = nullptr; 
		const std::type_info &ti = typeid(doubleptr);
		assertPrint("typeid().name() support 3/4:", ti.name());
		// CHECK: typeid().name() support 3/4: Pd
	}

	// Dereferencing a null pointer: okay for a non-polymorphic expression
	{
		double *doubleptr = nullptr;
		const std::type_info &ti = typeid(*doubleptr);
		assertPrint("typeid().name() support 4/4:", ti.name());
		// CHECK: typeid().name() support 4/4: d
		
	}
}

void testDynamicCast()
{
	I* a = new A();
	I* b = new B();
	I* t = dynamic_cast<A*>(a);

	if ((t = dynamic_cast<A*>(a))) {
		t->foo();
	}

	if ((t = dynamic_cast<B*>(a))) {
		t->bar();
	}

	assertPrint("A::foo is called:", (signed int)A_foo_called);
	// CHECK: A::foo is called: 1
	assertPrint("A::bar is called:", (signed int)A_bar_called);
	// CHECK: A::bar is called: 0
	assertPrint("B::foo is called:", (signed int)B_foo_called);
	// CHECK: B::foo is called: 0
	assertPrint("B::bar is called:", (signed int)B_bar_called);
	// CHECK: B::bar is called: 0

	if ((t = dynamic_cast<A*>(b))) {
		t->foo();
	}

	if ((t = dynamic_cast<B*>(b))) {
		t->bar();
	}

	assertPrint("A::foo is called:", (signed int)A_foo_called);
	// CHECK: A::foo is called: 1
	assertPrint("A::bar is called:", (signed int)A_bar_called);
	// CHECK: A::bar is called: 0
	assertPrint("B::foo is called:", (signed int)B_foo_called);
	// CHECK: B::foo is called: 1
	assertPrint("B::bar is called:", (signed int)B_bar_called);
	// CHECK: B::bar is called: 1

	void *v = dynamic_cast<void *>(a);
	assertPrint("Cast to void support 1/N:", (signed int)(v == a));
	// CHECK: Cast to void support 1/N: 1

	F* f = new F();
	I* volatile i = f;
	E* e = dynamic_cast<E*>(i);
	assertPrint("dynamic_Cast to empty non-primary base class:", e->getData());
	// CHECK: dynamic_Cast to empty non-primary base class: 42
}

void testMultipleInheritance()
{
	A* a = new A();
	D* d = new D();
	I* t;

	t = dynamic_cast<A*>(d);
	assertPrint("Multiple inheritance support 1/4:", (signed int)!t);
	// CHECK: Multiple inheritance support 1/4: 0

	t = dynamic_cast<C*>(d);
	assertPrint("Multiple inheritance support 2/4:", (signed int)!t);
	// CHECK: Multiple inheritance support 2/4: 0

	D* tmp = dynamic_cast<D*>(a);
	assertPrint("Multiple inheritance support 3/4:", (signed int)!tmp);
	// CHECK: Multiple inheritance support 3/4: 1

	// sidecast
	volatile C* c = dynamic_cast<volatile C*>(d);
	volatile A* va = dynamic_cast<volatile A*>(c);
	assertPrint("Multiple inheritance support 4/4:", (signed int)!va);
	// CHECK: Multiple inheritance support 4/4: 0
}

void webMain()
{
	testTypeidName();	
	testDynamicCast();
	testMultipleInheritance();
}

// vim: noexpandtab
