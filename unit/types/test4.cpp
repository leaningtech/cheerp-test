// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: Alignment at the end of class [3/3] : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2013 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>

class A
{
public:
	char a;
};

class B: public A
{
public:
	int b;
	B():b(0xdeadbeaf)
	{
		a=42;
	}
};

class C
{
public:
	short a;
	unsigned int b : 1;
	C():a(3),b(1)
	{
	}
};

void webMain()
{
	B b;

	assertEqual<char>(b.a, 42, "Alignment at the end of class [1/3]");
	assertEqual<int>(b.b, 0xdeadbeaf, "Alignment at the end of class [2/3]");

	volatile C c;
	assertEqual((short)c.a, (short)3, "Alignment at the end of class [3/3]");
}
