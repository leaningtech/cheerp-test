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

//If we need to pass functors as parameters, we need to tag [[cheerp::genercijs]]
[[cheerp::jsexport]][[cheerp::genericjs]]
int invokeFunction(int a, int b, int (*function)(int,int))
{
	return (*function)(a,b);
}

[[cheerp::jsexport]][[cheerp::genericjs]]
int invokeFunction2(int a, int b, int (*function)(int, int,int(*x)(int,int)))
{
	auto lambda = [](int a,int b) {return a+b;};
	return (*function)(a,b,lambda);
}

[[cheerp::genericjs]]
void webMain()
{
	int result;

	__asm__("invokeFunction(50, 50, (a,b)=>{return a+b;})" : "=r"(result) :);
	assertPrint("JSExport invoke functions 1/3:", result);
	// CHECK: JSExport invoke functions 1/3: 100

	__asm__("invokeFunction(50, 50, (a,b)=>{return a*b;})" : "=r"(result) :);
	assertPrint("JSExport invoke functions 2/3:", result);
	// CHECK: JSExport invoke functions 2/3: 2500

	__asm__("invokeFunction2(50, 50, invokeFunction)" : "=r"(result) :);
	assertPrint("JSExport invoke functions 3/3:", result);
	// CHECK: JSExport invoke functions 3/3: 100
}
