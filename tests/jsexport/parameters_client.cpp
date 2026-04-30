//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technlogies
//===----------------------------------------------------------------------===//

// REQUIRES: js, regular
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
// CHECK-NOT: FAILURE

#include <tests.h>

[[cheerp::jsexport]][[cheerp::genericjs]]
client::String* concatenateStrings(const client::String& a, const client::String& b)
{
	return a.concat(b);
}

[[cheerp::genericjs]]
void webMain()
{
	client::String* S;
	__asm__("concatenateStrings('Hello', ' World')" : "=r"(S) :);

	std::string cpp_string = (std::string)(*S);
	assertPrint("JSExport with client parameters and return:", cpp_string.c_str());
	// CHECK: JSExport with client parameters and return: Hello World
}
