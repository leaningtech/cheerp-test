//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// REQUIRES: wasm

// RUN: mkdir -p %t

// testing vanilla
// RUN: regular_only run_if_wasm compile_mode_wasm -pthread -o %t/vanilla %s 2>&1
// RUN: regular_only run_if_wasm python3 %S/../helpers/create_driver.py %t/vanilla %S/thread_setup.testing.js %t/vanilla_driver.js --module=vanilla --pthread
// RUN: regular_only run_if_wasm %node %t/vanilla_driver.js 2>&1 | %FileCheck %s

// testing es6
// RUN: regular_only run_if_wasm compile_mode_wasm -pthread -cheerp-make-module=es6 -o %t/es6.mjs %s 2>&1
// RUN: regular_only run_if_wasm python3 %S/../helpers/create_driver.py %t/es6.mjs %S/thread_setup.testing.js %t/es6_driver.js --module=es6 --pthread
// RUN: regular_only run_if_wasm %node %t/es6_driver.js 2>&1 | %FileCheck %s

// testing closure
// RUN: regular_only run_if_wasm compile_mode_wasm -pthread -cheerp-make-module=closure -o %t/closure %s 2>&1
// RUN: regular_only run_if_wasm python3 %S/../helpers/create_driver.py %t/closure %S/thread_setup.testing.js %t/closure_driver.js --module=closure --pthread
// RUN: regular_only run_if_wasm %node %t/closure_driver.js 2>&1 | %FileCheck %s

// testing genericjs
// RUN: regular_only run_if_wasm compile_mode_wasm -pthread -cheerp-make-module=commonjs -o %t/commonjs %s 2>&1
// RUN: regular_only run_if_wasm python3 %S/../helpers/create_driver.py %t/commonjs %S/thread_setup.testing.js %t/commonjs_driver.js --module=commonjs --pthread
// RUN: regular_only run_if_wasm %node %t/commonjs_driver.js 2>&1 | %FileCheck %s



#include <tests.h>

int testGlobal = 0;

[[cheerp::jsexport]]
int doCalculation(int amount)
{
	return amount * testGlobal;
}

void webMain()
{
	testGlobal = 5;
}

// CHECK: Module returned after thread setup : SUCCESS