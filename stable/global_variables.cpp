//===---------------------------------------------------------------------===//
//	Copyright 2024 Leaning Technlogies
//===----------------------------------------------------------------------===//

// Test global variables with jsexport across all targets
// RUN: mkdir -p %t
//
// Regular mode (PREEXECUTE_MODE=false) - Test all 4 module types (vanilla, ES6, CommonJS, closure):
// RUN: regular_only run_if_js %S/test_all_modules.sh %s %S/global_variables.testing.js %t compile_mode_js 2>&1 | %FileCheck %s 
//
// Also test wasm and asmjs with vanilla driver:
// RUN: regular_only run_if_wasm %valgrind compile_mode_wasm -o %t/w %s 2>&1
// RUN: regular_only run_if_wasm python3 %S/create_driver.py %t/w %S/global_variables.testing.js %t/w_driver.js --module=vanilla
// RUN: regular_only run_if_wasm %node %t/w_driver.js 2>&1 | %FileCheck %s
//
// RUN: regular_only run_if_asmjs %valgrind compile_mode_asmjs -o %t/a %s 2>&1
// RUN: regular_only run_if_asmjs python3 %S/create_driver.py %t/a %S/global_variables.testing.js %t/a_driver.js --module=vanilla
// RUN: regular_only run_if_asmjs %node %t/a_driver.js 2>&1 | %FileCheck %s 
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
