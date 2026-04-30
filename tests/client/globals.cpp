//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: regular, no-asan

// RUN: %compile -cheerp-make-module=commonjs -o %t-commonjs.js %s 2>&1
// RUN: python3 %helpers/create_driver.py %t-commonjs.js %S/globals.testing.js %t-commonjs_driver.js --module=commonjs
// RUN: %node %t-commonjs_driver.js 2>&1 | %FileCheck %s

// RUN: %compile -cheerp-make-module=es6 -o %t-es6.mjs %s 2>&1
// RUN: python3 %helpers/create_driver.py %t-es6.mjs %S/globals.testing.js %t-es6_driver.mjs --module=es6
// RUN: %node %t-es6_driver.mjs 2>&1 | %FileCheck %s


#include <tests.h>

namespace [[cheerp::genericjs]] client
{
	extern int externValue;
	int internValue = 100;

	class [[cheerp::client_layout]] SomeClass
	{
	public:
		SomeClass(int);
		void doTest(int);
	};
	extern SomeClass* externPtr;
	SomeClass* internPtr = new SomeClass(14);
	extern SomeClass externInstance;
}

[[cheerp::genericjs]]
void someFunc(client::SomeClass& instance)
{
	instance.doTest(33);
}

[[cheerp::genericjs]]
int main()
{
	assertPrint("Internal value 1/3: ", client::internValue);
	// CHECK: Internal value 1/3: 100

	client::internValue = 7;
	assertPrint("Internal value 2/3: ", client::internValue);
	// CHECK: Internal value 2/3: 7
	assertPrint("Internal value 3/3: ", client::internValue * client::internValue);
	// CHECK: Internal value 3/3: 49

	assertPrint("External value 1/3: ", client::externValue);
	// CHECK: External value 1/3: 100

	client::externValue = 7;
	assertPrint("External value 2/3: ", client::externValue);
	// CHECK: External value 2/3: 7

	assertPrint("External value 3/3: ", client::externValue * client::externValue);
	// CHECK: External value 3/3: 49
	
	client::internPtr->doTest(-42);
	client::externPtr->doTest(123);

	someFunc(client::externInstance);

	return 1;
}
