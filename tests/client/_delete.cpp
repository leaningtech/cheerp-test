//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: regular, no-asan

// RUN: %compile -cheerp-make-module=commonjs -o %t-commonjs.js %s 2>&1
// RUN: python3 %helpers/create_driver.py %t-commonjs.js %S/_delete.testing.js %t-commonjs_driver.js --module=commonjs
// RUN: %node %t-commonjs_driver.js 2>&1 | %FileCheck %s

// RUN: %compile -cheerp-make-module=es6 -o %t-es6.mjs %s 2>&1
// RUN: python3 %helpers/create_driver.py %t-es6.mjs %S/_delete.testing.js %t-es6_driver.mjs --module=es6
// RUN: %node %t-es6_driver.mjs 2>&1 | %FileCheck %s

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
