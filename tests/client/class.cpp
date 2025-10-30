//===---------------------------------------------------------------------===//
//	Copyright 2021 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t

// REQUIRES: regular

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 %then_run_js | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1 | %FileCheck %s

// CHECK: Static method: 0
// CHECK: Non static method: 14
// CHECK: Non static method: 21
// CHECK: Non static method: 28
// CHECK: Static method: 3


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

	assertPrint("Static method: ", client::SomeClass::someFunc());
	assertPrint("Non static method: ", c->otherFunc());
	assertPrint("Non static method: ", c->otherFunc());
	assertPrint("Non static method: ", c->otherFunc());
	assertPrint("Static method: ", client::SomeClass::someFunc());

	return 1;
}
