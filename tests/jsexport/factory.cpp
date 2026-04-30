//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technlogies
//===----------------------------------------------------------------------===//

// REQUIRES: regular

// Test vanilla module
// RUN: %compile -cheerp-reserved-names="X" -o %t-vanilla.js %s 2>&1
// RUN: %node %t-vanilla.js 2>&1 | %FileCheck %s

// Test ES6 module
// RUN: %compile -cheerp-reserved-names="X" -cheerp-make-module=es6 -o %t-es6.mjs %s 2>&1
// RUN: python3 %helpers/run_cheerp_module.py --module=es6 %t-es6.mjs 2>&1 | %FileCheck %s

// Test CommonJS module
// RUN: %compile -cheerp-reserved-names="X" -cheerp-make-module=commonjs -o %t-commonjs.js %s 2>&1
// RUN: python3 %helpers/run_cheerp_module.py --module=commonjs %t-commonjs.js 2>&1 | %FileCheck %s

// Test Closure module
// RUN: %compile -cheerp-reserved-names="X" -cheerp-make-module=closure -o %t-closure.js %s 2>&1
// RUN: %node %t-closure.js 2>&1 | %FileCheck %s

// Also test wasm and asmjs with vanilla driver:



#include <tests.h>

class [[cheerp::jsexport]][[cheerp::genericjs]] B
{
public:
	B(int k) : value(k)
	{
		counter++;
	}
	int getValue()
	{
		return value;
	}
	static int howMany()
	{
		return counter;
	}
private:
	int value;
	static int counter;
};

int B::counter = 0;

[[cheerp::jsexport]][[cheerp::genericjs]]
B* factoryB(int t)
{
	return new B(t);
}

class [[cheerp::jsexport]][[cheerp::genericjs]] A
{
public:
	A(int X) : value(X) {}
	B* generateB()
	{
		B* b = new B(value);
		return b;
	}
private:
	int value;
};

[[cheerp::genericjs]]B global(23);

[[cheerp::genericjs]]
void webMain()
{
	assertPrint("JSExport member function:", global.getValue());
	// CHECK: JSExport member function: 23

	A a(23);

	assertPrint("JSExport generator 1/4:", B::howMany());
	// CHECK: JSExport generator 1/4: 1
	B* generatedByA = a.generateB();
	assertPrint("JSExport generator 2/4:", B::howMany());
	// CHECK: JSExport generator 2/4: 2
	__asm__("var generator = new A(12); var X = generator.generateB(); X = factoryB(213);");
	assertPrint("JSExport generator 3/4:", B::howMany());
	// CHECK: JSExport generator 3/4: 4
	B* generatedByFactory = factoryB(1234);
	assertPrint("JSExport generator 4/4:", B::howMany());
	// CHECK: JSExport generator 4/4: 5
}
