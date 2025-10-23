// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: Nested namespace 1/4 : SUCCESS
// CHECK: Nested namespace 2/4 : SUCCESS
// CHECK: Nested namespace 3/4 : SUCCESS
// CHECK: Nested namespace 4/4 : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2021 Leaning Technlogies
//===----------------------------------------------------------------------===//

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
	assertEqual(counter, 1, "Nested namespace 1/4");

	client::first::someFunc();
	assertEqual(counter, 3, "Nested namespace 2/4");

	client::first::second::someFunc();
	assertEqual(counter, 7, "Nested namespace 3/4");

	client::first::second::third::someFunc();
	assertEqual(counter, 15, "Nested namespace 4/4");

	return 1;
}
