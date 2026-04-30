// REQUIRES: regular
//
// NOTE: This file is C, we only compile it (no JS harness), to ensure the attribute is accepted.
//
// Test vanilla module
// RUN: %run_if_js %compile -o %t-vanilla.js %s 2>&1 | %FileCheck %s
//
// Test ES6 module
// RUN: %run_if_js %compile -cheerp-make-module=es6 -o %t-es6.mjs %s 2>&1 | %FileCheck %s
//
// Test CommonJS module
// RUN: %run_if_js %compile -cheerp-make-module=commonjs -o %t-commonjs.js %s 2>&1 | %FileCheck %s
//
// Test Closure module
// RUN: %run_if_js %compile -cheerp-make-module=closure -o %t-closure.js %s 2>&1 | %FileCheck %s
//
// Also test wasm and asmjs compile:
// RUN: %run_if_wasm %compile -o %t-w %s 2>&1 | %FileCheck %s
// RUN: %run_if_asmjs %compile -o %t-a %s 2>&1 | %FileCheck %s

// CHECK-NOT: error
#include <stdint.h>

__attribute__((cheerp_jsexport_unsafe))
uint32_t
doSomething(uint32_t a)
{
	return a*a;
}
