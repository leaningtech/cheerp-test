//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: regular
// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s



#include <tests.h>
#include <cheerp/jsobject.h>

namespace [[cheerp::genericjs]] client
{
	void someFunc();

	struct first : public Object
	{
		static void someFunc();

	struct second : public Object
	{
		static void someFunc();

	struct third : public Object
	{
		static void someFunc();
	};
	};
	};
}

int counter = 0;

void [[cheerp::genericjs]][[cheerp::jsexport]] someFunc()
{
	counter += 1;
}

namespace first
{
	void [[cheerp::genericjs]][[cheerp::jsexport]] someFunc()
	{
		counter += 2;
	}

	namespace second
	{
	void [[cheerp::genericjs]][[cheerp::jsexport]] someFunc()
	{
		counter += 4;
	}
	namespace third
	{
	void [[cheerp::genericjs]][[cheerp::jsexport]] someFunc()
	{
		counter += 8;
	}
	}
	}
}

int [[cheerp::genericjs]] main()
{
	client::someFunc();
	assertPrint("Free standing function: ", counter);
	// CHECK: Free standing function: 1

	client::first::someFunc();
	assertPrint("Nested struct 1/3: ", counter);
	// CHECK: Nested struct 1/3: 3

	client::first::second::someFunc();
	assertPrint("Nested struct 2/3: ", counter);
	// CHECK: Nested struct 2/3: 7

	client::first::second::third::someFunc();
	assertPrint("Nested struct 3/3: ", counter);
	// CHECK: Nested struct 3/3: 15

	return 1;
}
