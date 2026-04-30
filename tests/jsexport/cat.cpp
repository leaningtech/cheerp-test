//===---------------------------------------------------------------------===//
//	Copyright 2022 Leaning Technologies
//===----------------------------------------------------------------------===//

// NOTE: ASAN is unsupported for JS target, see lit.cfg changes for details
// REQUIRES: regular


// Test vanilla module
// RUN: %compile -o %t-vanilla.js %s 2>&1
// RUN: python3 %helpers/create_driver.py %t-vanilla.js %S/cat.testing.js %t-vanilla_driver.js --module=vanilla
// RUN: %node %t-vanilla_driver.js 2>&1 | %FileCheck %s

// Test ES6 module
// RUN: %compile -cheerp-make-module=es6 -o %t-es6.mjs %s 2>&1
// RUN: python3 %helpers/create_driver.py %t-es6.mjs %S/cat.testing.js %t-es6_driver.mjs --module=es6
// RUN: %node %t-es6_driver.mjs 2>&1 | %FileCheck %s

// Test CommonJS module
// RUN: %compile -cheerp-make-module=commonjs -o %t-commonjs.js %s 2>&1
// RUN: python3 %helpers/create_driver.py %t-commonjs.js %S/cat.testing.js %t-commonjs_driver.js --module=commonjs
// RUN: %node %t-commonjs_driver.js 2>&1 | %FileCheck %s

// Test Closure module
// RUN: %compile -cheerp-make-module=closure -o %t-closure.js %s 2>&1
// RUN: python3 %helpers/create_driver.py %t-closure.js %S/cat.testing.js %t-closure_driver.js --module=closure
// RUN: %node %t-closure_driver.js 2>&1 | %FileCheck %s

// CHECK: Print something to console
// CHECK-NOT: unexpected exit : FAILURE

#include "./../tests.h"
#include <cheerp/client.h>

class [[cheerp::jsexport]] Cat {
public:
	[[cheerp::genericjs]]
	Cat(client::String* name) : age(name->operator int()) {}
	int age;

	[[cheerp::genericjs]]
	~Cat() {
		client::console.log("Print something to console");
	}
	int i0() {
		return age * age;
	}
};
