//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// REQUIRES: wasm, regular


// testing vanilla
// RUN: %run_if_wasm %compile -pthread -o %t-vanilla.js %s 2>&1
// RUN: %run_if_wasm python3 %helpers/create_driver.py %t-vanilla.js %S/thread_local.testing.js %t-vanilla_driver.js --module=vanilla --pthread
// RUN: %run_if_wasm %node %t-vanilla_driver.js 2>&1 | %FileCheck %s

// testing es6
// RUN: %run_if_wasm %compile -pthread -cheerp-make-module=es6 -o %t-es6.mjs %s 2>&1
// RUN: %run_if_wasm python3 %helpers/create_driver.py %t-es6.mjs %S/thread_local.testing.js %t-es6_driver.mjs --module=es6 --pthread
// RUN: %run_if_wasm %node %t-es6_driver.mjs 2>&1 | %FileCheck %s

// testing closure
// RUN: %run_if_wasm %compile -pthread -cheerp-make-module=closure -o %t-closure.js %s 2>&1
// RUN: %run_if_wasm python3 %helpers/create_driver.py %t-closure.js %S/thread_local.testing.js %t-closure_driver.js --module=closure --pthread
// RUN: %run_if_wasm %node %t-closure_driver.js 2>&1 | %FileCheck %s

// testing genericjs
// RUN: %run_if_wasm %compile -pthread -cheerp-make-module=commonjs -o %t-commonjs.js %s 2>&1
// RUN: %run_if_wasm python3 %helpers/create_driver.py %t-commonjs.js %S/thread_local.testing.js %t-commonjs_driver.js --module=commonjs --pthread
// RUN: %run_if_wasm %node %t-commonjs_driver.js 2>&1 | %FileCheck %s

#include <tests.h>

thread_local int myInt = 0;

struct myStruct {
	[[cheerp::genericjs]]
	myStruct(int param) : myVal(param)
	{
		myInt = param;
	}
	int myVal;
};

static thread_local struct myStruct myStr(7);

int main()
{
	assertPrint("thread local access in genericjs constructors:", myStr.myVal);
    // CHECK: thread local access in genericjs constructors: 7
}