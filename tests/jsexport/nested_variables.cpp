//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// UNSUPPORTED: asan
// NOTE: ASAN is unsupported for JS target, see lit.cfg changes for details

//
// Test vanilla module
// REQUIRES: regular
// RUN: %compile -o %t-vanilla.js %s 2>&1
// RUN: python3 %helpers/create_driver.py %t-vanilla.js %S/nested_variables.testing.js %t-vanilla_driver.js --module=vanilla
// RUN: %node %t-vanilla_driver.js 2>&1 | %FileCheck %s --check-prefix=CHECK

// Test ES6 module
// RUN: %compile -cheerp-make-module=es6 -o %t-es6.mjs %s 2>&1
// RUN: python3 %helpers/create_driver.py %t-es6.mjs %S/nested_variables.testing.js %t-es6_driver.mjs --module=es6
// RUN: %node %t-es6_driver.mjs 2>&1 | %FileCheck %s --check-prefix=CHECK

// Test CommonJS module
// RUN: %compile -cheerp-make-module=commonjs -o %t-commonjs.js %s 2>&1
// RUN: python3 %helpers/create_driver.py %t-commonjs.js %S/nested_variables.testing.js %t-commonjs_driver.js --module=commonjs
// RUN: %node %t-commonjs_driver.js 2>&1 | %FileCheck %s --check-prefix=CHECK

// Test Closure module
// RUN: %compile -cheerp-make-module=closure -o %t-closure.js %s 2>&1
// RUN: python3 %helpers/create_driver.py %t-closure.js %S/nested_variables.testing.js %t-closure_driver.js --module=closure
// RUN: %node %t-closure_driver.js 2>&1 | %FileCheck %s --check-prefix=CHECK
//
// Also test wasm and asmjs with vanilla driver:
//
//
// CHECK: CPP: nestedVariable (expect 1): 1
// CHECK: CPP: getNestedVariable() (expect 1): 1
// CHECK: CPP: nestedVariable (expect 2): 2
// CHECK: CPP: getNestedVariable() (expect 2): 2
// CHECK: CPP: nestedVariable (expect 3): 3
// CHECK: CPP: getNestedVariable() (expect 3): 3
// CHECK: JS: nestedVariable (expect 3): 3
// CHECK: JS: getNestedVariable() (expect 3): 3
// CHECK: JS: nestedVariable (expect 4): 4
// CHECK: JS: getNestedVariable() (expect 4): 4
// CHECK: JS: nestedVariable (expect 5): 5
// CHECK: JS: getNestedVariable() (expect 5): 5

#include <tests.h>

namespace Module {
	[[cheerp::jsexport]]
	int nestedVariable = 1;

	[[cheerp::jsexport]]
	int getNestedVariable() {
		return nestedVariable;
	}

	[[cheerp::jsexport]]
	void setNestedVariable(int value) {
		nestedVariable = value;
	}
}

int main() {

	__preexecute_print_case("CPP: nestedVariable (expect 1): ", Module::nestedVariable);
	__preexecute_print_case("CPP: getNestedVariable() (expect 1):", Module::getNestedVariable());
	Module::nestedVariable = 2;
	__preexecute_print_case("CPP: nestedVariable (expect 2): ", Module::nestedVariable);
	__preexecute_print_case("CPP: getNestedVariable() (expect 2):", Module::getNestedVariable());
	Module::setNestedVariable(3);
	__preexecute_print_case("CPP: nestedVariable (expect 3): ", Module::nestedVariable);
	__preexecute_print_case("CPP: getNestedVariable() (expect 3):", Module::getNestedVariable());
	
}
