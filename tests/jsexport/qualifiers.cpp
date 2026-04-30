//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technlogies
//===----------------------------------------------------------------------===//

// REQUIRES: regular
//
// Test vanilla module
// RUN: %run_if_js %compile -o %t-vanilla.js %s 2>&1
// RUN: %run_if_js %node %t-vanilla.js 2>&1 | %FileCheck %s
//
// Test ES6 module
// RUN: %run_if_js %compile -cheerp-make-module=es6 -o %t-es6.mjs %s 2>&1
// RUN: %run_if_js python3 %helpers/run_cheerp_module.py --module=es6 %t-es6.mjs 2>&1 | %FileCheck %s
//
// Test CommonJS module
// RUN: %run_if_js %compile -cheerp-make-module=commonjs -o %t-commonjs.js %s 2>&1
// RUN: %run_if_js python3 %helpers/run_cheerp_module.py --module=commonjs %t-commonjs.js 2>&1 | %FileCheck %s
//
// Test Closure module
// RUN: %run_if_js %compile -cheerp-make-module=closure -o %t-closure.js %s 2>&1
// RUN: %run_if_js %node %t-closure.js 2>&1 | %FileCheck %s
//
// Also test wasm and asmjs with vanilla driver:
// RUN: %run_if_wasm %compile -o %t-w %s 2>&1
// RUN: %run_if_wasm %node %t-w 2>&1 | %FileCheck %s
//
// RUN: %run_if_asmjs %compile -o %t-a %s 2>&1
// RUN: %run_if_asmjs %node %t-a 2>&1 | %FileCheck %s

// CHECK-NOT: FAILURE

#include <tests.h>
#include <cheerp/clientlib.h>

[[cheerp::jsexport]] [[cheerp::genericjs]]
void foo(const int, const client::String&, const client::String*) {}

[[cheerp::genericjs]]
void webMain()
{
	client::String* s = new client::String("x");
	foo(0, *s, s);
	assertPrint("JSExport qualifiers 1/1");
	// CHECK: JSExport qualifiers 1/1
}
