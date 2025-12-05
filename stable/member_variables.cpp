//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//
//
// Test member variables with jsexport across all targets
// RUN: mkdir -p %t
//
// Regular mode (PREEXECUTE_MODE=false) - Test all 4 module types (vanilla, ES6, CommonJS, closure):
// RUN: regular_only run_if_js %S/test_all_modules.sh %s %S/member_variables.testing.js %t compile_mode_js 2>&1 | %FileCheck %s 
//
// Also test wasm and asmjs with vanilla driver:
// RUN: regular_only run_if_wasm %valgrind compile_mode_wasm -o %t/w %s 2>&1
// RUN: regular_only run_if_wasm python3 %S/create_driver.py %t/w %S/member_variables.testing.js %t/w_driver.js --module=vanilla
// RUN: regular_only run_if_wasm %node %t/w_driver.js 2>&1 | %FileCheck %s
//
// RUN: regular_only run_if_asmjs %valgrind compile_mode_asmjs -o %t/a %s 2>&1
// RUN: regular_only run_if_asmjs python3 %S/create_driver.py %t/a %S/member_variables.testing.js %t/a_driver.js --module=vanilla
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
