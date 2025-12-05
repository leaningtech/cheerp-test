//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// Test static variables with jsexport across all targets
// RUN: mkdir -p %t
//
// Regular mode (PREEXECUTE_MODE=false) - Test all 4 module types (vanilla, ES6, CommonJS, closure):
// RUN: regular_only run_if_js %S/test_all_modules.sh %s %S/static_variables.testing.js %t compile_mode_js 2>&1 | %FileCheck %s 
//
// Also test wasm and asmjs with vanilla driver:
// RUN: regular_only run_if_wasm %valgrind compile_mode_wasm -o %t/w %s 2>&1
// RUN: regular_only run_if_wasm python3 %S/create_driver.py %t/w %S/static_variables.testing.js %t/w_driver.js --module=vanilla
// RUN: regular_only run_if_wasm %node %t/w_driver.js 2>&1 | %FileCheck %s
//
// RUN: regular_only run_if_asmjs %valgrind compile_mode_asmjs -o %t/a %s 2>&1
// RUN: regular_only run_if_asmjs python3 %S/create_driver.py %t/a %S/static_variables.testing.js %t/a_driver.js --module=vanilla
// RUN: regular_only run_if_asmjs %node %t/a_driver.js 2>&1 | %FileCheck %s 
//
// CHECK: CPP: staticVariable (expect 1): 1
// CHECK: CPP: getStaticVariable() (expect 1): 1
// CHECK: CPP: staticVariable (expect 2): 2
// CHECK: CPP: getStaticVariable() (expect 2): 2
// CHECK: CPP: staticVariable (expect 3): 3
// CHECK: CPP: getStaticVariable() (expect 3): 3
// CHECK: JS: staticVariable (expect 3): 3
// CHECK: JS: getStaticVariable() (expect 3): 3
// CHECK: JS: staticVariable (expect 4): 4
// CHECK: JS: getStaticVariable() (expect 4): 4
// CHECK: JS: staticVariable (expect 5): 5
// CHECK: JS: getStaticVariable() (expect 5): 5

#include <tests.h>
struct [[cheerp::jsexport]] Struct {
	[[cheerp::jsexport]]
	static int staticVariable;

	static int getStaticVariable() {
		return staticVariable;
	}

	static void setStaticVariable(int value) {
		staticVariable = value;
	}
};

int Struct::staticVariable = 1;

int main() {

	__preexecute_print_case("CPP: staticVariable (expect 1): ", Struct::staticVariable);
	__preexecute_print_case("CPP: getStaticVariable() (expect 1):", Struct::getStaticVariable());
	Struct::staticVariable = 2;
	__preexecute_print_case("CPP: staticVariable (expect 2): ", Struct::staticVariable);
	__preexecute_print_case("CPP: getStaticVariable() (expect 2):", Struct::getStaticVariable());
	Struct::setStaticVariable(3);
	__preexecute_print_case("CPP: staticVariable (expect 3): ", Struct::staticVariable);
	__preexecute_print_case("CPP: getStaticVariable() (expect 3):", Struct::getStaticVariable());

}