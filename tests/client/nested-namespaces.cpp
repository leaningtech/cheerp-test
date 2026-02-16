//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 %then_run_js | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1 | %FileCheck %s

// CHECK: Nested namespace 1/4: 1 
// CHECK: Nested namespace 2/4: 3
// CHECK: Nested namespace 3/4: 7
// CHECK: Nested namespace 4/4: 15

#include <tests.h>

namespace [[cheerp::genericjs]] client
{
	void someFunc();

	namespace [[cheerp::genericjs]] first
	{
	void someFunc();

	namespace [[cheerp::genericjs]] second
	{
	void someFunc();

	namespace [[cheerp::genericjs]] third
	{
	void someFunc();
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
	assertPrint("Nested namespace 1/4: ", counter);

	client::first::someFunc();
	assertPrint("Nested namespace 2/4: ", counter);

	client::first::second::someFunc();
	assertPrint("Nested namespace 3/4: ", counter);

	client::first::second::third::someFunc();
	assertPrint("Nested namespace 4/4: ", counter);

	return 1;
}
