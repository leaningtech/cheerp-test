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

	assertPrint("Cast member function pointer to primary base 1/3:", (A(1).*fA)(42));
	// CHECK: Cast member function pointer to primary base 1/3: 43
	assertPrint("Cast member function pointer to primary base 2/3:", (B(2).*fB)(42));
	// CHECK: Cast member function pointer to primary base 2/3: 54
	assertPrint("Cast member function pointer to primary base 3/3:", (C(3).*fC)(42));
	// CHECK: Cast member function pointer to primary base 3/3: 65
	FP volatile fE = static_cast<FP>(&E::func);
	FPE volatile fE2 = &E::func;
	FP volatile fE3 = static_cast<FP>(fE2);
	FP volatile fE4 = getMemberPointer();
	assertPrint("Cast member function pointer to not primary base 1/3:", (E().*fE)(42));
	// CHECK: Cast member function pointer to not primary base 1/3: 172
	assertPrint("Cast member function pointer to not primary base 1/3:", (E().*fE3)(43));
	// CHECK: Cast member function pointer to not primary base 1/3: 173
	assertPrint("Cast member function pointer to not primary base 1/3:", (E().*fE4)(44));
	// CHECK: Cast member function pointer to not primary base 1/3: 174

	FP volatile fV = static_cast<FP>(&A::funcV);
	assertPrint("Virtual member function pointer:", (C(3).*fV)(43));
	// CHECK: Virtual member function pointer: 66

	FP volatile fP = unitBlackBox(1)? static_cast<FP>(&A::funcV) : getMemberPointer();
	assertPrint("Virtual member function pointer with PHI:", (C(3).*fP)(43));
	// CHECK: Virtual member function pointer with PHI: 66
}
