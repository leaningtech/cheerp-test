//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: asan
// NOTE: ASAN is unsupported for JS target, see lit.cfg changes for details

// RUN: mkdir -p %t
//
// Test vanilla module
// RUN: regular_only run_if_js compile_mode_js -o %t/vanilla.js %s 2>&1
// RUN: regular_only run_if_js python3 %S/../helpers/create_driver.py %t/vanilla.js %S/member_variables.testing.js %t/vanilla_driver.js --module=vanilla
// RUN: regular_only run_if_js %node %t/vanilla_driver.js 2>&1 | %FileCheck %s --check-prefix=CHECK

// Test ES6 module
// RUN: regular_only run_if_js compile_mode_js -cheerp-make-module=es6 -o %t/es6.mjs %s 2>&1
// RUN: regular_only run_if_js python3 %S/../helpers/create_driver.py %t/es6.mjs %S/member_variables.testing.js %t/es6_driver.mjs --module=es6
// RUN: regular_only run_if_js %node %t/es6_driver.mjs 2>&1 | %FileCheck %s --check-prefix=CHECK

// Test CommonJS module
// RUN: regular_only run_if_js compile_mode_js -cheerp-make-module=commonjs -o %t/commonjs.js %s 2>&1
// RUN: regular_only run_if_js python3 %S/../helpers/create_driver.py %t/commonjs.js %S/member_variables.testing.js %t/commonjs_driver.js --module=commonjs
// RUN: regular_only run_if_js %node %t/commonjs_driver.js 2>&1 | %FileCheck %s --check-prefix=CHECK

// Test Closure module
// RUN: regular_only run_if_js compile_mode_js -cheerp-make-module=closure -o %t/closure.js %s 2>&1
// RUN: regular_only run_if_js python3 %S/../helpers/create_driver.py %t/closure.js %S/member_variables.testing.js %t/closure_driver.js --module=closure
// RUN: regular_only run_if_js %node %t/closure_driver.js 2>&1 | %FileCheck %s --check-prefix=CHECK
//
// Also test wasm and asmjs with vanilla driver:
// RUN: regular_only run_if_wasm %valgrind compile_mode_wasm -o %t/w %s 2>&1
// RUN: regular_only run_if_wasm python3 %S/../helpers/create_driver.py %t/w %S/member_variables.testing.js %t/w_driver.js --module=vanilla
// RUN: regular_only run_if_wasm %node %t/w_driver.js 2>&1 | %FileCheck %s
//
// RUN: regular_only run_if_asmjs %valgrind compile_mode_asmjs -o %t/a %s 2>&1
// RUN: regular_only run_if_asmjs python3 %S/../helpers/create_driver.py %t/a %S/member_variables.testing.js %t/a_driver.js --module=vanilla
// RUN: regular_only run_if_asmjs %node %t/a_driver.js 2>&1 | %FileCheck %s 
//
// CHECK: CPP: memberVariable (expect 1): 1
// CHECK: CPP: getMemberVariable (expect 1): 1
// CHECK: CPP: memberVariable (expect 2): 2
// CHECK: CPP: getMemberVariable (expect 2): 2
// CHECK: CPP: memberVariable (expect 3): 3
// CHECK: CPP: getMemberVariable (expect 3): 3
// CHECK: JS: memberVariable (expect 3): 3
// CHECK: JS: getMemberVariable (expect 3): 3
// CHECK: JS: memberVariable (expect 4): 4
// CHECK: JS: getMemberVariable (expect 4): 4
// CHECK: JS: memberVariable (expect 5): 5
// CHECK: JS: getMemberVariable (expect 5): 5

#include <tests.h>

struct [[cheerp::jsexport]] Struct {
	[[cheerp::jsexport]]
	int memberVariable = 1;

	int getMemberVariable() {
		return memberVariable;
	}

	void setMemberVariable(int value) {
		memberVariable = value;
	}
};

Struct* instance = new Struct();

[[cheerp::jsexport]]
Struct* getInstance() {
	return instance;
}

int main() {

	__preexecute_print_case("CPP: memberVariable (expect 1): ", instance->memberVariable);
	__preexecute_print_case("CPP: getMemberVariable (expect 1): ", instance->getMemberVariable());
	instance->memberVariable = 2;
	__preexecute_print_case("CPP: memberVariable (expect 2): ", instance->memberVariable);
	__preexecute_print_case("CPP: getMemberVariable (expect 2): ", instance->getMemberVariable());
	instance->setMemberVariable(3);
	__preexecute_print_case("CPP: memberVariable (expect 3): ", instance->memberVariable);
	__preexecute_print_case("CPP: getMemberVariable (expect 3): ", instance->getMemberVariable());

}
