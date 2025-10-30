//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: regular, no-asan

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -cheerp-make-module=commonjs -o %t/j_commonjs.js %s 2>&1
// RUN: regular_only run_if_js python3 %S/../helpers/create_driver.py %t/j_commonjs.js %S/_delete.testing.js %t/j_commonjs_driver.js --module=commonjs
// RUN: regular_only run_if_js %node %t/j_commonjs_driver.js 2>&1 | %FileCheck %s

// RUN: regular_only run_if_js compile_mode_js -cheerp-make-module=es6 -o %t/j_es6.mjs %s 2>&1
// RUN: regular_only run_if_js python3 %S/../helpers/create_driver.py %t/j_es6.mjs %S/_delete.testing.js %t/j_es6_driver.mjs --module=es6
// RUN: regular_only run_if_js %node %t/j_es6_driver.mjs 2>&1 | %FileCheck %s

// wasm + commonjs
// RUN: regular_only run_if_wasm compile_mode_wasm -cheerp-make-module=commonjs -o %t/w_commonjs.js %s 2>&1
// RUN: regular_only run_if_wasm python3 %S/../helpers/create_driver.py %t/w_commonjs.js %S/_delete.testing.js %t/w_commonjs_driver.js --module=commonjs
// RUN: regular_only run_if_wasm %node %t/w_commonjs_driver.js 2>&1 | %FileCheck %s

// wasm + es6
// RUN: regular_only run_if_wasm compile_mode_wasm -cheerp-make-module=es6 -o %t/w_es6.mjs %s 2>&1
// RUN: regular_only run_if_wasm python3 %S/../helpers/create_driver.py %t/w_es6.mjs %S/_delete.testing.js %t/w_es6_driver.mjs --module=es6
// RUN: regular_only run_if_wasm %node %t/w_es6_driver.mjs 2>&1 | %FileCheck %s

// asmjs + commonjs
// RUN: regular_only run_if_asmjs compile_mode_asmjs -cheerp-make-module=commonjs -o %t/a_commonjs.js %s 2>&1
// RUN: regular_only run_if_asmjs python3 %S/../helpers/create_driver.py %t/a_commonjs.js %S/_delete.testing.js %t/a_commonjs_driver.js --module=commonjs
// RUN: regular_only run_if_asmjs %node %t/a_commonjs_driver.js 2>&1 | %FileCheck %s

// asmjs + es6
// RUN: regular_only run_if_asmjs compile_mode_asmjs -cheerp-make-module=es6 -o %t/a_es6.mjs %s 2>&1
// RUN: regular_only run_if_asmjs python3 %S/../helpers/create_driver.py %t/a_es6.mjs %S/_delete.testing.js %t/a_es6_driver.mjs --module=es6
// RUN: regular_only run_if_asmjs %node %t/a_es6_driver.mjs 2>&1 | %FileCheck %s

#include "./../tests.h"

namespace [[cheerp::genericjs]] client
{
	class [[cheerp::client_layout]] ToBeDeleted
	{
	public:
		ToBeDeleted(int a);
		[[cheerp::interface_name(("delete"))]] void _delete();
		[[cheerp::interface_name(("catch"))]] int _catch();
	};
	int counterAlive();
	template <class T>
	[[cheerp::interface_name(("assert"))]]
	void _assert(T& someObject);
}

[[cheerp::genericjs]]
int main()
{
	assertPrint("Check counter initialization: ", client::counterAlive());
	// CHECK: Check counter initialization: 0
	client::ToBeDeleted* TBD = new client::ToBeDeleted(123);
	client::_assert<client::ToBeDeleted>(*TBD);
	assertPrint("Check counter increment: ", client::counterAlive());
	// CHECK: Check counter increment: 1
	assertPrint("Call actual catch: ", TBD->_catch());
	// CHECK: Call actual catch: 123
	
	TBD -> _delete();

	assertPrint("Check counter decrement: ", client::counterAlive());
	// CHECK: Check counter decrement: 0
}
