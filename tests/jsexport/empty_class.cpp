//===---------------------------------------------------------------------===//
//	Copyright 2022 Leaning Technologies
//===----------------------------------------------------------------------===//

// Test empty classes with jsexport across all targets
//
// UNSUPPORTED: asan
// NOTE: ASAN is unsupported for JS target, see lit.cfg changes for details

//
// Test vanilla module
// REQUIRES: regular
// RUN: %compile -o %t-vanilla.js %s 2>&1
// RUN: python3 %helpers/create_driver.py %t-vanilla.js %S/empty_class.testing.js %t-vanilla_driver.js --module=vanilla
// RUN: %node %t-vanilla_driver.js 2>&1 | %FileCheck %s --check-prefix=CHECK

// Test ES6 module
// RUN: %compile -cheerp-make-module=es6 -o %t-es6.mjs %s 2>&1
// RUN: python3 %helpers/create_driver.py %t-es6.mjs %S/empty_class.testing.js %t-es6_driver.mjs --module=es6
// RUN: %node %t-es6_driver.mjs 2>&1 | %FileCheck %s --check-prefix=CHECK

// Test CommonJS module
// RUN: %compile -cheerp-make-module=commonjs -o %t-commonjs.js %s 2>&1
// RUN: python3 %helpers/create_driver.py %t-commonjs.js %S/empty_class.testing.js %t-commonjs_driver.js --module=commonjs
// RUN: %node %t-commonjs_driver.js 2>&1 | %FileCheck %s --check-prefix=CHECK

// Test Closure module
// RUN: %compile -cheerp-make-module=closure -o %t-closure.js %s 2>&1
// RUN: python3 %helpers/create_driver.py %t-closure.js %S/empty_class.testing.js %t-closure_driver.js --module=closure
// RUN: %node %t-closure_driver.js 2>&1 | %FileCheck %s --check-prefix=CHECK
//
// Also test wasm and asmjs with vanilla driver:
//
//
// CHECK: can construct new Empty(): false
// CHECK: can construct new AllPrivate(): false
// CHECK: can construct new OnlyStatic(): false

class [[cheerp::jsexport]] Empty {};

class [[cheerp::jsexport]] AllPrivate {
	AllPrivate() {}
	void func() { var++; }
	int var;
};

class [[cheerp::jsexport]] OnlyStatic {
public:
	static void func() { var++; }
	static int var;
};

int OnlyStatic::var = 0;

int main() {
	Empty empty;
	OnlyStatic onlyStatic;
}
