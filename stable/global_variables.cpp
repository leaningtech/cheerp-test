// Test global variables with jsexport across all targets
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
// CHECK: CPP: 1 == globalVariable : SUCCESS
// CHECK: CPP: 1 == getGlobalVariable() : SUCCESS
// CHECK: CPP: 2 == globalVariable : SUCCESS
// CHECK: CPP: 2 == getGlobalVariable() : SUCCESS
// CHECK: CPP: 3 == globalVariable : SUCCESS
// CHECK: CPP: 3 == getGlobalVariable() : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2024 Leaning Technologies
//===----------------------------------------------------------------------===//

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
	assertEqual(1, globalVariable, "CPP: 1 == globalVariable");
	assertEqual(1, getGlobalVariable(), "CPP: 1 == getGlobalVariable()");
	globalVariable = 2;
	assertEqual(2, globalVariable, "CPP: 2 == globalVariable");
	assertEqual(2, getGlobalVariable(), "CPP: 2 == getGlobalVariable()");
	setGlobalVariable(3);
	assertEqual(3, globalVariable, "CPP: 3 == globalVariable");
	assertEqual(3, getGlobalVariable(), "CPP: 3 == getGlobalVariable()");
}
