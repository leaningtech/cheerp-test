//===---------------------------------------------------------------------===//
//	Copyright 2021 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: regular
// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s



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
