// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: Nearly empty virtual bases 1/1 : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2018 Leaning Technlogies
//===---------------------------------------------------------------------===//

#include <tests.h>

class A {
public:
	virtual ~A() { }
};

class B : virtual public A {
};

class C : public B, virtual public A {
public:
	virtual int foo();
};

int C::foo()
{
	return 1;
}
void webMain()
{
	C c;
	int ret = c.foo();
	assertEqual(1, ret, "Nearly empty virtual bases 1/1");
}
