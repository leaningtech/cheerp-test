//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// UNSUPPORTED: asan
// NOTE: ASAN is unsupported for JS target, see lit.cfg changes for details

// RUN: mkdir -p %t
//
// Test vanilla module
// RUN: regular_only run_if_js compile_mode_js -o %t/vanilla.js %s 2>&1
// RUN: regular_only run_if_js python3 %S/../helpers/create_driver.py %t/vanilla.js %S/inheritance.testing.js %t/vanilla_driver.js --module=vanilla
// RUN: regular_only run_if_js %node %t/vanilla_driver.js 2>&1 | %FileCheck %s --check-prefix=CHECK

// Test ES6 module
// RUN: regular_only run_if_js compile_mode_js -cheerp-make-module=es6 -o %t/es6.mjs %s 2>&1
// RUN: regular_only run_if_js python3 %S/../helpers/create_driver.py %t/es6.mjs %S/inheritance.testing.js %t/es6_driver.mjs --module=es6
// RUN: regular_only run_if_js %node %t/es6_driver.mjs 2>&1 | %FileCheck %s --check-prefix=CHECK

// Test CommonJS module
// RUN: regular_only run_if_js compile_mode_js -cheerp-make-module=commonjs -o %t/commonjs.js %s 2>&1
// RUN: regular_only run_if_js python3 %S/../helpers/create_driver.py %t/commonjs.js %S/inheritance.testing.js %t/commonjs_driver.js --module=commonjs
// RUN: regular_only run_if_js %node %t/commonjs_driver.js 2>&1 | %FileCheck %s --check-prefix=CHECK

// Test Closure module
// RUN: regular_only run_if_js compile_mode_js -cheerp-make-module=closure -o %t/closure.js %s 2>&1
// RUN: regular_only run_if_js python3 %S/../helpers/create_driver.py %t/closure.js %S/inheritance.testing.js %t/closure_driver.js --module=closure
// RUN: regular_only run_if_js %node %t/closure_driver.js 2>&1 | %FileCheck %s --check-prefix=CHECK
//
// Also test wasm and asmjs with vanilla driver:
// RUN: regular_only run_if_wasm %valgrind compile_mode_wasm -o %t/w %s 2>&1
// RUN: regular_only run_if_wasm python3 %S/../helpers/create_driver.py %t/w %S/inheritance.testing.js %t/w_driver.js --module=vanilla
// RUN: regular_only run_if_wasm %node %t/w_driver.js 2>&1 | %FileCheck %s
//
// RUN: regular_only run_if_asmjs %valgrind compile_mode_asmjs -o %t/a %s 2>&1
// RUN: regular_only run_if_asmjs python3 %S/../helpers/create_driver.py %t/a %S/inheritance.testing.js %t/a_driver.js --module=vanilla
// RUN: regular_only run_if_asmjs %node %t/a_driver.js 2>&1 | %FileCheck %s 
//
// CHECK: base memberBaseTest (expect 1): 1
// CHECK: base memberShadowTest (expect 0): 0
// CHECK: base virtualTest (expect 0): 0
// CHECK: base pureVirtualTest (expect 0): 0

// CHECK: derived memberBaseTest (expect 1): 1
// CHECK: derived memberDerivedTest (expect 1): 1
// CHECK: derived memberShadowTest (expect 1): 1
// CHECK: derived virtualTest (expect 1): 1
// CHECK: derived pureVirtualTest (expect 1): 1
//
// CHECK: base staticMemberTest (expect 1): 1
// CHECK: base StaticShadowTest (expect 0): 0
//
// CHECK: derived staticDerivedTest (expect 1): 1
// CHECK: derived staticShadowTest (expect 1): 1
//
// CHECK: base instanceof Derived (expect false): false
// CHECK: derived instanceof Base (expect true): true


struct [[cheerp::jsexport]] Interface {
	virtual int pureVirtualTest() = 0;

	virtual ~Interface() {}
};

struct [[cheerp::jsexport]] Base : Interface {
	Base() {}

	int memberBaseTest() { return 1; }
	int memberShadowTest() { return 0; }
	virtual int virtualTest() { return 0; }
	int pureVirtualTest() override { return 0; }

	static int staticBaseTest() { return 1; }
	static int staticShadowTest() { return 0; }
};

struct [[cheerp::jsexport]] Derived : Base {
	Derived() {}

	int memberDerivedTest() { return 1; }
	int memberShadowTest() { return 1; }
	int virtualTest() override { return 1; }
	int pureVirtualTest() override { return 1; }
	
	static int staticDerivedTest() { return 1; }
	static int staticShadowTest() { return 1; }
};
