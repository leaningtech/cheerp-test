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
// RUN: regular_only run_if_js python3 %helpers/run_cheerp_module.py --module=es6 %t/es6.mjs 2>&1 | %FileCheck %s
//
// Test CommonJS module
// RUN: regular_only run_if_js compile_mode_js -cheerp-make-module=commonjs -o %t/commonjs.js %s 2>&1
// RUN: regular_only run_if_js python3 %helpers/run_cheerp_module.py --module=commonjs %t/commonjs.js 2>&1 | %FileCheck %s
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

[[cheerp::jsexport]]
int sumInt(int a, int b)
{
	return a+b;
}

[[cheerp::jsexport]]
int sumUInt(unsigned int a, unsigned int b)
{
	return a+b;
}

[[cheerp::jsexport]]
int sumChar(char a, char b)
{
	//Return has to be int
	return a+b;
}

[[cheerp::jsexport]]
int sumUChar(unsigned char a, unsigned char b)
{
	//Return has to be int
	return a+b;
}

[[cheerp::jsexport]]
double sumDouble(double a, double b)
{
	return a+b;
}

[[cheerp::jsexport]]
float sumFloat(float a, float b)
{
	return a+b;
}

[[cheerp::jsexport]]
bool areEqualChar(char a, char b)
{
	return a==b;
}

int global_variable{0};

[[cheerp::jsexport]]
void setToFortyTwo()
{
	assertPrint("JSExport free function void(void) 1/2:", global_variable);
	global_variable = 42;
}

[[cheerp::jsexport]][[cheerp::genericjs]]
void setToFortyTwoGenericJS()
{
	assertPrint("JSExport free function void(void) 2/2:", global_variable);
	global_variable = 42;
}

[[cheerp::genericjs]]
void webMain()
{
	int result;

	__asm__("sumInt(-101,100)" : "=r"(result) :);
	assertPrint("JSExport free function int:", result);
	// CHECK: JSExport free function int: -1

	__asm__("sumUInt(-101,100)" : "=r"(result) :);
	assertPrint("JSExport free function unsigned int:", result);
	// CHECK: JSExport free function unsigned int: -1

	__asm__("sumChar(-128,127)" : "=r"(result) :);
	assertPrint("JSExport free function char:", result);
	// CHECK: JSExport free function char: -1

	__asm__("sumUChar(-128,127)" : "=r"(result) :);
	assertPrint("JSExport free function unsigned char:", result);
	// CHECK: JSExport free function unsigned char: 255

	__asm__("areEqualChar(12,13)" : "=r"(result) :);
	assertPrint("JSExport free function returns bool:", result);
	// CHECK: JSExport free function returns bool: 0

	global_variable = 23;
	// CHECK: JSExport free function void(void) 1/2: 23
	setToFortyTwo();
	assertPrint("JSExport free function void(void) 2/2:", global_variable);
	// CHECK: JSExport free function void(void) 2/2: 42

	global_variable = 23;
	// JSExport free function void(void) genericjs 1/2: 23
	setToFortyTwoGenericJS();
	assertPrint("JSExport free function void(void) genericjs 2/2:", global_variable);
	// JSExport free function void(void) genericjs 1/2: 42
}
