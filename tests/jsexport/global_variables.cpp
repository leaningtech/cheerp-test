//===---------------------------------------------------------------------===//
//	Copyright 2024 Leaning Technlogies
//===----------------------------------------------------------------------===//

// UNSUPPORTED: asan
// NOTE: ASAN is unsupported for JS target, see lit.cfg changes for details

//
// Test vanilla module
// REQUIRES: regular
// RUN: %compile -o %t-vanilla.js %s 2>&1
// RUN: python3 %helpers/create_driver.py %t-vanilla.js %S/global_variables.testing.js %t-vanilla_driver.js --module=vanilla
// RUN: %node %t-vanilla_driver.js 2>&1 | %FileCheck %s --check-prefix=CHECK

// Test ES6 module
// RUN: %compile -cheerp-make-module=es6 -o %t-es6.mjs %s 2>&1
// RUN: python3 %helpers/create_driver.py %t-es6.mjs %S/global_variables.testing.js %t-es6_driver.mjs --module=es6
// RUN: %node %t-es6_driver.mjs 2>&1 | %FileCheck %s --check-prefix=CHECK

// Test CommonJS module
// RUN: %compile -cheerp-make-module=commonjs -o %t-commonjs.js %s 2>&1
// RUN: python3 %helpers/create_driver.py %t-commonjs.js %S/global_variables.testing.js %t-commonjs_driver.js --module=commonjs
// RUN: %node %t-commonjs_driver.js 2>&1 | %FileCheck %s --check-prefix=CHECK

// Test Closure module
// RUN: %compile -cheerp-make-module=closure -o %t-closure.js %s 2>&1
// RUN: python3 %helpers/create_driver.py %t-closure.js %S/global_variables.testing.js %t-closure_driver.js --module=closure
// RUN: %node %t-closure_driver.js 2>&1 | %FileCheck %s --check-prefix=CHECK
//
// Also test wasm and asmjs with vanilla driver:
//
//
// CHECK: CPP: globalVariable (expect 1): 1
// CHECK: CPP: getGlobalVariable() (expect 1): 1
// CHECK: CPP: globalVariable (expect 2): 2
// CHECK: CPP: getGlobalVariable() (expect 2): 2
// CHECK: CPP: globalVariable (expect 3): 3
// CHECK: CPP: getGlobalVariable() (expect 3): 3
// CHECK: JS: getGlobalVariable() (expect 3): 3
// CHECK: JS: getGlobalVariable() (expect 4): 4
// CHECK: JS: getGlobalVariable() (expect 5): 5


#include <tests.h>

[[cheerp::jsexport]]
int globalVariable = 1;

[[cheerp::jsexport]]
int getGlobalVariable() {
	return globalVariable;
}

[[cheerp::jsexport]]
void setGlobalVariable(int value) {
	globalVariable = value;
}

int main() {
	__preexecute_print_case("CPP: globalVariable (expect 1): ", globalVariable);
	__preexecute_print_case("CPP: getGlobalVariable() (expect 1): ", getGlobalVariable());
	globalVariable = 2;
	__preexecute_print_case("CPP: globalVariable (expect 2): ", globalVariable);
	__preexecute_print_case("CPP: getGlobalVariable() (expect 2): ", getGlobalVariable());
	setGlobalVariable(3);
	__preexecute_print_case("CPP: globalVariable (expect 3): ", globalVariable);
	__preexecute_print_case("CPP: getGlobalVariable() (expect 3): ", getGlobalVariable());

}
