// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: Static method : SUCCESS
// CHECK: Non static method : SUCCESS
// CHECK: Non static method : SUCCESS
// CHECK: Non static method : SUCCESS
// CHECK: Static method : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2021 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>
#include <cheerp/jsobject.h>

namespace [[cheerp::genericjs]] client
{
	struct SomeClass : public Object
	{
		SomeClass(int X);
		static int someFunc();
		int otherFunc();
	};
}

class [[cheerp::jsexport]][[cheerp::genericjs]] SomeClass
{
public:
	SomeClass(int Y)
		: counter(Y)
	{
	}
	static int someFunc()
	{
		return invocations;
	}
	int otherFunc()
	{
		invocations++;
		counter += 7;
		return counter;
	}
private:
	int counter;
	static int invocations;
};

int SomeClass::invocations = 0;

int [[cheerp::genericjs]] main()
{
	client::SomeClass* c = new client::SomeClass(7);

	assertEqual(client::SomeClass::someFunc(), 0, "Static method");

	assertEqual(c->otherFunc(), 14, "Non static method");
	assertEqual(c->otherFunc(), 21, "Non static method");
	assertEqual(c->otherFunc(), 28, "Non static method");

	assertEqual(client::SomeClass::someFunc(), 3, "Static method");

	return 1;
}
