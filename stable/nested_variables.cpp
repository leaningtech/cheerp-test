//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t
//
// Regular mode (PREEXECUTE_MODE=false) - Test all 4 module types (vanilla, ES6, CommonJS, closure):
// RUN: regular_only run_if_js %S/test_all_modules.sh %s %S/nested_variables.testing.js %t compile_mode_js 2>&1 | %FileCheck %s 
//
// Also test wasm and asmjs with vanilla driver:
// RUN: regular_only run_if_wasm %valgrind compile_mode_wasm -o %t/w %s 2>&1
// RUN: regular_only run_if_wasm python3 %S/create_driver.py %t/w %S/nested_variables.testing.js %t/w_driver.js --module=vanilla
// RUN: regular_only run_if_wasm %node %t/w_driver.js 2>&1 | %FileCheck %s
//
// RUN: regular_only run_if_asmjs %valgrind compile_mode_asmjs -o %t/a %s 2>&1
// RUN: regular_only run_if_asmjs python3 %S/create_driver.py %t/a %S/nested_variables.testing.js %t/a_driver.js --module=vanilla
// RUN: regular_only run_if_asmjs %node %t/a_driver.js 2>&1 | %FileCheck %s 
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
