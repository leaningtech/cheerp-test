//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// REQUIRES: wasm, regular


// testing vanilla
// RUN: %run_if_wasm %compile -pthread -o %t-vanilla.js %s 2>&1
// RUN: %run_if_wasm python3 %helpers/create_driver.py %t-vanilla.js %S/atomic_common.testing.js %t-vanilla_driver.js --module=vanilla --pthread
// RUN: %run_if_wasm %node %t-vanilla_driver.js 2>&1 | %FileCheck %s

// testing es6
// RUN: %run_if_wasm %compile -pthread -cheerp-make-module=es6 -o %t-es6.mjs %s 2>&1
// RUN: %run_if_wasm python3 %helpers/create_driver.py %t-es6.mjs %S/atomic_common.testing.js %t-es6_driver.mjs --module=es6 --pthread
// RUN: %run_if_wasm %node %t-es6_driver.mjs 2>&1 | %FileCheck %s

// testing closure
// RUN: %run_if_wasm %compile -pthread -cheerp-make-module=closure -o %t-closure.js %s 2>&1
// RUN: %run_if_wasm python3 %helpers/create_driver.py %t-closure.js %S/atomic_common.testing.js %t-closure_driver.js --module=closure --pthread
// RUN: %run_if_wasm %node %t-closure_driver.js 2>&1 | %FileCheck %s

// testing genericjs
// RUN: %run_if_wasm %compile -pthread -cheerp-make-module=commonjs -o %t-commonjs.js %s 2>&1
// RUN: %run_if_wasm python3 %helpers/create_driver.py %t-commonjs.js %S/atomic_common.testing.js %t-commonjs_driver.js --module=commonjs --pthread
// RUN: %run_if_wasm %node %t-commonjs_driver.js 2>&1 | %FileCheck %s


#include <tests.h>
#include <atomic>

std::atomic<int> value(0);

void addToNumber()
{
	int num = value.load();
	int newNum = num + 1;
	while (!value.compare_exchange_weak(num, newNum))
		newNum = num + 1;
}

int main()
{
	for (int i = 0; i < 100; i++)
		addToNumber();
	assertPrint("Simple compare exchange:", value.load());
	// CHECK: Simple compare exchange: 100
}
