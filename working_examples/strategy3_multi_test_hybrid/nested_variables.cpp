// Test nested variables with jsexport across all targets
// RUN: mkdir -p %t
// Preexecution mode (PREEXECUTE_MODE=true):
// RUN: compile_for_preexec_js -O1 -o %t/j_pre %s 2>&1 && echo "Preexecution compile successful"
// RUN: compile_for_preexec_asmjs -O1 -o %t/a_pre %s 2>&1 && echo "Preexecution compile successful"
//
// Regular mode (PREEXECUTE_MODE=false):
// RUN: compile_for_wasm -O1 -o %t/w %s
// RUN: compile_for_js -O1 -o %t/j %s
// RUN: compile_for_asmjs -O1 -o %t/a %s
//
// RUN: run_if_wasm test -f %t/w && test -f %t/w.wasm
// RUN: run_if_js test -f %t/j
// RUN: run_if_asmjs test -f %t/a
//
// RUN: run_if_wasm %node %t/w 2>&1 | %FileCheck %s --check-prefix=CHECK
// RUN: run_if_js %node %t/j 2>&1 | %FileCheck %s --check-prefix=CHECK
// RUN: run_if_asmjs %node %t/a 2>&1 | %FileCheck %s --check-prefix=CHECK
//
// CHECK: CPP: 1 == nestedVariable : SUCCESS
// CHECK: CPP: 1 == getNestedVariable() : SUCCESS
// CHECK: CPP: 2 == nestedVariable : SUCCESS
// CHECK: CPP: 2 == getNestedVariable() : SUCCESS
// CHECK: CPP: 3 == nestedVariable : SUCCESS
// CHECK: CPP: 3 == getNestedVariable() : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2024 Leaning Technologies
//===----------------------------------------------------------------------===//

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
	assertEqual(1, Module::nestedVariable, "CPP: 1 == nestedVariable");
	assertEqual(1, Module::getNestedVariable(), "CPP: 1 == getNestedVariable()");
	Module::nestedVariable = 2;
	assertEqual(2, Module::nestedVariable, "CPP: 2 == nestedVariable");
	assertEqual(2, Module::getNestedVariable(), "CPP: 2 == getNestedVariable()");
	Module::setNestedVariable(3);
	assertEqual(3, Module::nestedVariable, "CPP: 3 == nestedVariable");
	assertEqual(3, Module::getNestedVariable(), "CPP: 3 == getNestedVariable()");
}
