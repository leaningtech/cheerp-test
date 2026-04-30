//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technlogies
//===----------------------------------------------------------------------===//

// REQUIRES: regular
//
// Test vanilla module
// RUN: %compile -o %t-vanilla.js %s 2>&1
// RUN: %node %t-vanilla.js 2>&1 | %FileCheck %s
//
// Test ES6 module
// RUN: %compile -cheerp-make-module=es6 -o %t-es6.mjs %s 2>&1
// RUN: python3 %helpers/run_cheerp_module.py --module=es6 %t-es6.mjs 2>&1 | %FileCheck %s
//
// Test CommonJS module
// RUN: %compile -cheerp-make-module=commonjs -o %t-commonjs.js %s 2>&1
// RUN: python3 %helpers/run_cheerp_module.py --module=commonjs %t-commonjs.js 2>&1 | %FileCheck %s
//
// Test Closure module
// RUN: %compile -cheerp-make-module=closure -o %t-closure.js %s 2>&1
// RUN: %node %t-closure.js 2>&1 | %FileCheck %s
//
// Also test wasm and asmjs with vanilla driver:
//

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
