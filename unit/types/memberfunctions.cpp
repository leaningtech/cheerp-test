// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: Cast member function pointer to primary base 1/3 : SUCCESS
// CHECK: Cast member function pointer to primary base 2/3 : SUCCESS
// CHECK: Cast member function pointer to primary base 3/3 : SUCCESS
// CHECK: Cast member function pointer to not primary base 1/3 : SUCCESS
// CHECK: Cast member function pointer to not primary base 2/3 : SUCCESS
// CHECK: Cast member function pointer to not primary base 3/3 : SUCCESS
// CHECK: Virtual member function pointer : SUCCESS
// CHECK: Virtual member function pointer with PHI : SUCCESS

//===----------------------------------------------------------------------===//
//	Copyright 2017 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>

struct A
{
	int baseVal;
	// TODO: TypeOptimizer issue here, the base class is collapsed
	double hack;
	A(int v):baseVal(v)
	{
	}
	int funcBase(int a)
	{
		return baseVal+a;
	}
	// Adding this virtual function break the error on casting member pointer to non-primary base class
	virtual int funcV(int a)
	{
		return 0;
	}
};

struct B: public A
{
	float derivedVal;
	B(int v):A(10),derivedVal(v)
	{
	}
	int func(int a)
	{
		return baseVal+derivedVal+a;
	}
};

struct C: public A
{
	int derivedVal;
	C(int v):A(20),derivedVal(v)
	{
	}
	int func(int a)
	{
		return baseVal+derivedVal+a;
	}
	int funcV(int a) override
	{
		return func(a);
	}
};

struct D
{
	int i[2];
};

struct E: public D, public A
{
	E():A(30)
	{
		i[0] = 100;
		i[1] = 101;
	}
	int func(int a)
	{
		return baseVal+i[0]+a;
	}
};

typedef int(A::*FP)(int);
typedef int(E::*FPE)(int);

FP __attribute__((noinline)) getMemberPointer()
{
	return static_cast<FP>(&E::func);
}

void webMain()
{
	// Test simple linear inheritance
	FP volatile fA = static_cast<FP>(&A::funcBase);
	FP volatile fB = static_cast<FP>(&B::func);
	FP volatile fC = static_cast<FP>(&C::func);

	assertEqual((A(1).*fA)(42), 43, "Cast member function pointer to primary base 1/3");
	assertEqual((B(2).*fB)(42), 54, "Cast member function pointer to primary base 2/3");
	assertEqual((C(3).*fC)(42), 65, "Cast member function pointer to primary base 3/3");

	FP volatile fE = static_cast<FP>(&E::func);
	FPE volatile fE2 = &E::func;
	FP volatile fE3 = static_cast<FP>(fE2);
	FP volatile fE4 = getMemberPointer();
	assertEqual((E().*fE)(42), 172, "Cast member function pointer to not primary base 1/3");
	assertEqual((E().*fE3)(43), 173, "Cast member function pointer to not primary base 2/3");
	assertEqual((E().*fE4)(44), 174, "Cast member function pointer to not primary base 3/3");

	FP volatile fV = static_cast<FP>(&A::funcV);
	assertEqual((C(3).*fV)(43), 66, "Virtual member function pointer");

	FP volatile fP = unitBlackBox(1)? static_cast<FP>(&A::funcV) : getMemberPointer();
	assertEqual((C(3).*fP)(43), 66, "Virtual member function pointer with PHI");
}
