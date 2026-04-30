// REQUIRES: regular
//
// NOTE: This file is C, we only compile it (no JS harness), to ensure the attribute is accepted.
//
// Test vanilla module
// RUN: %compile -o %t-vanilla.js %s 2>&1 | %FileCheck %s
//
// Test ES6 module
// RUN: %compile -cheerp-make-module=es6 -o %t-es6.mjs %s 2>&1 | %FileCheck %s
//
// Test CommonJS module
// RUN: %compile -cheerp-make-module=commonjs -o %t-commonjs.js %s 2>&1 | %FileCheck %s
//
// Test Closure module
// RUN: %compile -cheerp-make-module=closure -o %t-closure.js %s 2>&1 | %FileCheck %s
//
// Also test wasm and asmjs compile:

// CHECK-NOT: error
#include <stdint.h>

__attribute__((cheerp_jsexport_unsafe))
uint32_t
doSomething(uint32_t a)
{
	return a*a;
}
