//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technlogies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t
// REQUIRES: regular
//
// Test vanilla module
// RUN: regular_only run_if_js compile_mode_js -o %t/vanilla.js %s 2>&1
// RUN: regular_only run_if_js %node %t/vanilla.js 2>&1 | %FileCheck %s
//
// Test ES6 module
// RUN: regular_only run_if_js compile_mode_js -cheerp-make-module=es6 -o %t/es6.mjs %s 2>&1
// RUN: regular_only run_if_js python3 %S/../helpers/run_cheerp_module.py --module=es6 %t/es6.mjs 2>&1 | %FileCheck %s
//
// Test CommonJS module
// RUN: regular_only run_if_js compile_mode_js -cheerp-make-module=commonjs -o %t/commonjs.js %s 2>&1
// RUN: regular_only run_if_js python3 %S/../helpers/run_cheerp_module.py --module=commonjs %t/commonjs.js 2>&1 | %FileCheck %s
//
// Test Closure module
// RUN: regular_only run_if_js compile_mode_js -cheerp-make-module=closure -o %t/closure.js %s 2>&1
// RUN: regular_only run_if_js %node %t/closure.js 2>&1 | %FileCheck %s
//
// Also test wasm and asmjs with vanilla driver:
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1
// RUN: regular_only run_if_wasm %node %t/w 2>&1 | %FileCheck %s
//
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1
// RUN: regular_only run_if_asmjs %node %t/a 2>&1 | %FileCheck %s

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
