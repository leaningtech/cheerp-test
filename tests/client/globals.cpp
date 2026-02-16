//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: regular, no-asan

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -cheerp-make-module=commonjs -o %t/j_commonjs.js %s 2>&1
// RUN: regular_only run_if_js python3 %S/../helpers/create_driver.py %t/j_commonjs.js %S/globals.testing.js %t/j_commonjs_driver.js --module=commonjs
// RUN: regular_only run_if_js %node %t/j_commonjs_driver.js 2>&1 | %FileCheck %s

// RUN: regular_only run_if_js compile_mode_js -cheerp-make-module=es6 -o %t/j_es6.mjs %s 2>&1
// RUN: regular_only run_if_js python3 %S/../helpers/create_driver.py %t/j_es6.mjs %S/globals.testing.js %t/j_es6_driver.mjs --module=es6
// RUN: regular_only run_if_js %node %t/j_es6_driver.mjs 2>&1 | %FileCheck %s

// wasm + commonjs
// RUN: regular_only run_if_wasm compile_mode_wasm -cheerp-make-module=commonjs -o %t/w_commonjs.js %s 2>&1
// RUN: regular_only run_if_wasm python3 %S/../helpers/create_driver.py %t/w_commonjs.js %S/globals.testing.js %t/w_commonjs_driver.js --module=commonjs
// RUN: regular_only run_if_wasm %node %t/w_commonjs_driver.js 2>&1 | %FileCheck %s

// wasm + es6
// RUN: regular_only run_if_wasm compile_mode_wasm -cheerp-make-module=es6 -o %t/w_es6.mjs %s 2>&1
// RUN: regular_only run_if_wasm python3 %S/../helpers/create_driver.py %t/w_es6.mjs %S/globals.testing.js %t/w_es6_driver.mjs --module=es6
// RUN: regular_only run_if_wasm %node %t/w_es6_driver.mjs 2>&1 | %FileCheck %s

// asmjs + commonjs
// RUN: regular_only run_if_asmjs compile_mode_asmjs -cheerp-make-module=commonjs -o %t/a_commonjs.js %s 2>&1
// RUN: regular_only run_if_asmjs python3 %S/../helpers/create_driver.py %t/a_commonjs.js %S/globals.testing.js %t/a_commonjs_driver.js --module=commonjs
// RUN: regular_only run_if_asmjs %node %t/a_commonjs_driver.js 2>&1 | %FileCheck %s

// asmjs + es6
// RUN: regular_only run_if_asmjs compile_mode_asmjs -cheerp-make-module=es6 -o %t/a_es6.mjs %s 2>&1
// RUN: regular_only run_if_asmjs python3 %S/../helpers/create_driver.py %t/a_es6.mjs %S/globals.testing.js %t/a_es6_driver.mjs --module=es6
// RUN: regular_only run_if_asmjs %node %t/a_es6_driver.mjs 2>&1 | %FileCheck %s


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
