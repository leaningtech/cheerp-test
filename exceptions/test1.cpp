//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: regular

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -fexceptions -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck --check-prefixes=CHECK-JS,CHECK %s
// RUN: regular_only run_if_wasm compile_mode_wasm -fexceptions -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -fexceptions -o %t/a %s 2>&1 && node %t/a 2>&1 | %FileCheck %s 

#include <tests.h>
#include <cheerpintrin.h>
#include <cheerp/jsexception.h>

struct Never
{
};

struct A
{
	static int countC;
	static int countD;
	int i;
	A(int i): i(i)
	{
		countC++;
	}
	A(const A& o): i(o.i)
	{
		countC++;
	}
	virtual ~A()
	{
		countD++;
	}
};
int A::countC = 0;
int A::countD = 0;

struct B
{
	static int countC;
	static int countD;
	char c;
	B(char c): c(c)
	{
		countC++;
	}
	B(const B& o): c(o.c)
	{
		countC++;
	}
	virtual ~B()
	{
		countD++;
	}
};
int B::countC = 0;
int B::countD = 0;

struct C
{
	static int countC;
	static int countD;
	short s;
	C(short s): s(s)
	{
		countC++;
	}
	C(const C& o): s(o.s)
	{
		countC++;
	}
	virtual ~C()
	{
		countD++;
	}
};
int C::countC = 0;
int C::countD = 0;

struct X: public A, public B, public virtual C
{
	static int countC;
	static int countD;
	int i;
	X(int i): A(i), B(i), C(i), i(i)
	{
		countC++;
	}
	X(const X& o): A(o.i), B(o.i), C(o.i), i(o.i)
	{
		countC++;
	}
	virtual ~X()
	{
		countD++;
	}
};
int X::countC = 0;
int X::countD = 0;

struct Y: public virtual C
{
	static int countC;
	static int countD;
	short s;
	Y(short s): C(s), s(s)
	{
		countC++;
	}
	Y(const Y& o): C(o.s), s(o.s)
	{
		countC++;
	}
	virtual ~Y()
	{
		countD++;
	}
};
int Y::countC = 0;
int Y::countD = 0;

struct Z: public X, public Y
{
	static int countC;
	static int countD;
	char c;
	Z(char c): X(c), Y(c), C(3), c(c)
	{
		countC++;
	}
	Z(const Z& o): X(o.c), Y(o.c), C(o.C::s), c(o.c)
	{
		countC++;
	}
	virtual ~Z()
	{
		countD++;
	}
};
int Z::countC = 0;
int Z::countD = 0;

void zeroCounters()
{
	A::countC = 0;
	A::countD = 0;
	B::countC = 0;
	B::countD = 0;
	C::countC = 0;
	C::countD = 0;
	X::countC = 0;
	X::countD = 0;
	Y::countC = 0;
	Y::countD = 0;
	Z::countC = 0;
	Z::countD = 0;
}

template<typename T, int k>
void throwIf(T t, int v)
{
	if(v == k)
		throw t;
	else
		throw Never{};
}

void testBasic()
{
	int i = unitBlackBox(10);
	int r = 0;

	try {
		throwIf<int, 10>(i, i);
		r = 1;
	} catch(int e) {
		r = e;
	}
	assertPrint("Throw basic type 1/2:", r); 
	//CHECK: Throw basic type 1/2: 10
	assertPrint("Throw basic type 2/2:", (r == i)); 
	//CHECK: Throw basic type 2/2: 1
}

void testPtr()
{
	int i = unitBlackBox(10);
	int j = unitBlackBox(11);
	int* r = nullptr;

	try {
		throwIf<int*, 10>(&i, i);
		r = &j;
	} catch(int* e) {
		r = e;
	}
	assertPrint("Throw pointer type 1/2:", r ==&i); 
	//CHECK: Throw pointer type 1/2: 1
	assertPrint("Throw pointer type 2/2:", *r); 
	//CHECK: Throw pointer type 2/2: 10
}

void testObject()
{
	zeroCounters();

	int i = unitBlackBox(10);
	A a(i);
	int r = 0;

	try {
		throwIf<A, 10>(a, a.i);
		r = 1;
	} catch(A a) {
		r = a.i;
	}
	assertPrint("Throw object type 1/4:", (r == a.i)); 
	//CHECK: Throw object type 1/4: 1
	assertPrint("Throw object type 2/4:", r); 
	//CHECK: Throw object type 2/4: 10
	assertPrint("Throw object type 3/4:", a.countC); 
	//CHECK: Throw object type 3/4: 4
	assertPrint("Throw object type 4/4:", a.countD); 
	//CHECK: Throw object type 4/4: 3

}

void testObjectRef()
{
	zeroCounters();

	int i = unitBlackBox(10);
	A a(i);
	int r = 0;

	try {
		throwIf<A, 10>(a, a.i);
		r = 1;
	} catch(A& a) {
		r = a.i;
	}
	assertPrint("Throw object type ref 1/4:", (r == a.i)); 
	//CHECK: Throw object type ref 1/4: 1 
	assertPrint("Throw object type ref 2/4:", r); 
	//CHECK: Throw object type ref 2/4: 10
	assertPrint("Throw object type ref 3/4:", a.countC); 
	//CHECK: Throw object type ref 3/4: 3
	assertPrint("Throw object type ref 4/4:", a.countD); 
	//CHECK: Throw object type ref 4/4: 2
}

void testObjectPtr()
{
	zeroCounters();

	int i = unitBlackBox(10);
	A* a = new A(i);
	int r = 0;

	try {
		throwIf<A*, 10>(a, a->i);
		r = 1;
	} catch(A* a) {
		r = a->i;
	}
	assertPrint("Throw object type ptr 1/4:", (r == a->i)); 
	//CHECK: Throw object type ptr 1/4: 1
	delete a;
	assertPrint("Throw object type ptr 2/4:", r); 
	//CHECK: Throw object type ptr 2/4: 10
	assertPrint("Throw object type ptr 3/4:", a->countC); 
	//CHECK: Throw object type ptr 3/4: 1
	assertPrint("Throw object type ptr 4/4:", a->countD); 
	//CHECK: Throw object type ptr 4/4: 1
}

void testObjectRefBase()
{
	zeroCounters();

	int i = unitBlackBox(10);
	X x(i);
	int r = 0;

	try {
		throwIf<X, 10>(x, x.i);
		r = 1;
	} catch(B& b) {
		r = b.c;
	} catch(X& x) {
		r = -1;
	}
	assertPrint("Throw object type ptr and catch by base type 1/4:", (r == (int)x.c)); 
	//CHECK: Throw object type ptr and catch by base type 1/4: 1
	assertPrint("Throw object type ptr and catch by base type 2/4:", r); 
	//CHECK: Throw object type ptr and catch by base type 2/4: 10
	assertPrint("Throw object type ptr and catch by base type 3/4:", x.countC); 
	//CHECK: Throw object type ptr and catch by base type 3/4: 3
	assertPrint("Throw object type ptr and catch by base type 4/4:", x.countD); 
	//CHECK: Throw object type ptr and catch by base type 4/4: 2
}
void testObjectPtrBase()
{
	zeroCounters();

	int i = unitBlackBox(10);
	X* x = new X(i);
	int r = 0;

	try {
		throwIf<X*, 10>(x, x->i);
		r = 1;
	} catch(B* b) {
		r = b->c;
	} catch(X* b) {
		r = -1;
	}
	assertPrint("Throw object type ptr and catch by base type pointer 1/4:", (r == (int)x->c)); 
	//CHECK: Throw object type ptr and catch by base type pointer 1/4: 1
	delete x;
	assertPrint("Throw object type ptr and catch by base type pointer 2/4:", r); 
	//CHECK: Throw object type ptr and catch by base type pointer 2/4: 10
	assertPrint("Throw object type ptr and catch by base type pointer 3/4:", X::countC); 
	//CHECK: Throw object type ptr and catch by base type pointer 3/4: 1
	assertPrint("Throw object type ptr and catch by base type pointer 4/4:", X::countD); 
	//CHECK: Throw object type ptr and catch by base type pointer 4/4: 1
}

void testObjectRefVBase()
{
	zeroCounters();

	int i = unitBlackBox(10);
	Z z(i);
	int r = 0;

	try {
		throwIf<Z, 10>(z, z.c);
		r = 1;
	} catch(C& c) {
		r = c.s;
	} catch(X& x) {
		r = -1;
	}
	assertPrint("Throw object type ptr and catch by virtual base type 1/4:", (r == (int)z.C::s));
	//CHECK: Throw object type ptr and catch by virtual base type 1/4: 1
	assertPrint("Throw object type ptr and catch by virtual base type 2/4:", r);
	//CHECK: Throw object type ptr and catch by virtual base type 2/4: 3
	assertPrint("Throw object type ptr and catch by virtual base type 3/4:", z.countC);
	//CHECK: Throw object type ptr and catch by virtual base type 3/4: 3
	assertPrint("Throw object type ptr and catch by virtual base type 4/4:", z.countD);
	//CHECK: Throw object type ptr and catch by virtual base type 4/4: 2
}

void testObjectPtrVBase()
{
	zeroCounters();

	int i = unitBlackBox(10);
	Z* z = new Z(i);
	int r = 0;

	try {
		throwIf<Z*, 10>(z, z->c);
		r = 1;
	} catch(C* c) {
		r = c->s;
	} catch(X* x) {
		r = -1;
	}
	assertPrint("Throw object type ptr and catch by virtual base type pointer 1/4:", (r == (int)z->C::s));
	//CHECK: Throw object type ptr and catch by virtual base type pointer 1/4: 1
	delete z;
	assertPrint("Throw object type ptr and catch by virtual base type pointer 2/4:", r);
	//CHECK: Throw object type ptr and catch by virtual base type pointer 2/4: 3
	assertPrint("Throw object type ptr and catch by virtual base type pointer 3/4:", Z::countC);
	//CHECK: Throw object type ptr and catch by virtual base type pointer 3/4: 1
	assertPrint("Throw object type ptr and catch by virtual base type pointer 4/4:", Z::countD);
	//CHECK: Throw object type ptr and catch by virtual base type pointer 4/4: 1
}

void testNullptr()
{
	zeroCounters();

	int i = unitBlackBox(10);
	int r = 0;

	try {
		throwIf<C*, 10>(nullptr, i);
		r = 1;
	} catch(A* a) {
		r = -1;
	} catch(C* c) {
		r = 2;
	}
	assertPrint("Throw a null pointer and catch:", r);
	//CHECK: Throw a null pointer and catch: 2
}

void testNullptr_t()
{
	zeroCounters();

	int i = unitBlackBox(10);
	int r = 0;

	try {
		throwIf<std::nullptr_t, 10>(nullptr, i);
		r = 1;
	} catch(std::nullptr_t n) {
		r = -1;
	} catch(C* c) {
		r = c->s;
	}
	assertPrint("Throw nullptr_t type and catch it:", r); 
	//CHECK: Throw nullptr_t type and catch it: -1
}

#ifndef __ASMJS__
void testJsObject()
{
	int i = unitBlackBox(10);
	client::Object* o = new client::Object();
	o->set_("data", i); 
	int r = 0;

	try {
		__builtin_cheerp_throw(o);
		r = 1;
	} catch(cheerp::JSException& e) {
		r = int(*(*e.get())["data"]);
	} catch(X* x) {
		r = -1;
	}
	assertPrint("Throw a client::Object* and catch it:", (r == i));
	//CHECK-JS: Throw a client::Object* and catch it: 1
}
#endif

void testRethrow()
{
	zeroCounters();

	int i = unitBlackBox(10);
	A a(i);
	int r = 0;

	auto inner = [](A& a, int& r)
	{
		try {
			throwIf<A, 10>(a, a.i);
			r = 1;
		} catch(A& a) {
			r = a.i;
			throw;
		}
	};
	try {
		inner(a, r);
	} catch(A& a) {
		r+= a.i;
	}
	assertPrint("Throw object type rethrow 1/4:", (r == 2*a.i));
	//CHECK: Throw object type rethrow 1/4: 1
	assertPrint("Throw object type rethrow 2/4:", r);
	//CHECK: Throw object type rethrow 2/4: 20
	assertPrint("Throw object type rethrow 3/4:", a.countC);
	//CHECK: Throw object type rethrow 3/4: 3
	assertPrint("Throw object type rethrow 4/4:", a.countD);
	//CHECK: Throw object type rethrow 4/4: 2
}

#ifndef __ASMJS__
void testRethrowForeign()
{
	int i = unitBlackBox(10);
	client::Object* o = new client::Object();
	o->set_("data", i); 
	int r = 0;

	auto inner = [](client::Object* o, int& r)
	{
		try {
			__builtin_cheerp_throw(o);
			r = 1;
		} catch(cheerp::JSException& e) {
			r = int(*(*e.get())["data"]);
			throw;
		} catch(X* x) {
			r = -1;
		}
	};

	try {
		inner(o, r);
	} catch(cheerp::JSException& e) {
		r += int(*(*e.get())["data"]);
	}
	assertPrint("Throw a client::Object* and rethrow it:", (r == 2*i));
	//CHECK-JS: Throw a client::Object* and rethrow it: 1
}
#endif

void testResume()
{
	zeroCounters();

	int i = unitBlackBox(10);
	A a(i);
	int r = 0;

	auto inner = [](A& a, int& r)
	{
		try {
			throwIf<A, 10>(a, a.i);
			r = 1;
		} catch(B& b) {
			r = b.c;
		}
		r = 2;
	};
	try {
		inner(a, r);
	} catch(A& a) {
		r+= a.i;
	}
	assertPrint("Test resume unwinding 1/4:", (r == a.i));
	//CHECK: Test resume unwinding 1/4: 1
	assertPrint("Test resume unwinding 2/4:", r);
	//CHECK: Test resume unwinding 2/4: 10
	assertPrint("Test resume unwinding 3/4:", a.countC);
	//CHECK: Test resume unwinding 3/4: 3
	assertPrint("Test resume unwinding 4/4:", a.countD);
	//CHECK: Test resume unwinding 4/4: 2
}

void testGetCurrentException()
{
	zeroCounters();

	int i = unitBlackBox(10);
	A a(i);
	int r = 0;

	auto inner = [](A& a, int& r)
	{
		std::exception_ptr ex;
		try {
			throwIf<A, 10>(a, a.i);
			r = 1;
		} catch(A& a) {
			ex = std::current_exception();
			r = a.i;
		}
		return ex;
	};
	try {
		auto ex = inner(a, r);
		std::rethrow_exception(ex);
	} catch(A& a) {
		r+= a.i;
	}
	assertPrint("Capture and rethrow current exception 1/4:", (r == 2*a.i));
	//CHECK: Capture and rethrow current exception 1/4: 1
	assertPrint("Capture and rethrow current exception 2/4:", r);
	//CHECK: Capture and rethrow current exception 2/4: 20
	assertPrint("Capture and rethrow current exception 3/4:", a.countC);
	//CHECK: Capture and rethrow current exception 3/4: 3
	assertPrint("Capture and rethrow current exception 4/4:", a.countD);
	//CHECK: Capture and rethrow current exception 4/4: 2
}

struct T
{
	static int count;
	int i;
	T(int i): i(i)
	{
	}
	~T()
	{
		A a(i);
		try {
			throwIf<A, 10>(a, a.i);
		} catch (A& a) {
			count = std::uncaught_exceptions();
		}
	}
};
int T::count = 0;

void testNestedUnwind()
{
	zeroCounters();

	int i = unitBlackBox(10);
	try {
		T t(i);
		throwIf<int, 10>(i, i);
		T::count = -1;
	} catch(int i) {
	}
	assertPrint("Throw exception during another exception cleanup:", T::count);
	//CHECK: Throw exception during another exception cleanup: 1
}

void testCatchAll()
{
	int i = unitBlackBox(10);
	int r = 0;
	try {
		throwIf<int, 10>(i, i);
		r = -1;
	} catch(...) {
		r = 1;
	}
	assertPrint("Catchall clause:", r);
	//CHECK: Catchall clause: 1
}

void webMain()
{
	testBasic();
	testPtr();
	testObject();
	testObjectRef();
	testObjectPtr();
	testObjectRefBase();
	testObjectPtrBase();
	testObjectRefVBase();
	testObjectPtrVBase();
	testNullptr();
	testNullptr_t();
#ifndef __ASMJS__
	testJsObject();
#endif
	testRethrow();
#ifndef __ASMJS__
	testRethrowForeign();
#endif
	testResume();
	testGetCurrentException();
	testNestedUnwind();
	testCatchAll();
}
