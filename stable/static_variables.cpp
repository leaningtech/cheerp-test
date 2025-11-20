// Test static variables with jsexport across all targets
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
// CHECK: CPP: 1 == staticVariable : SUCCESS
// CHECK: CPP: 1 == getStaticVariable() : SUCCESS
// CHECK: CPP: 2 == staticVariable : SUCCESS
// CHECK: CPP: 2 == getStaticVariable() : SUCCESS
// CHECK: CPP: 3 == staticVariable : SUCCESS
// CHECK: CPP: 3 == getStaticVariable() : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2024 Leaning Technologies
//===----------------------------------------------------------------------===//

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
	assertEqual(1, Struct::staticVariable, "CPP: 1 == staticVariable");
	assertEqual(1, Struct::getStaticVariable(), "CPP: 1 == getStaticVariable()");
	Struct::staticVariable = 2;
	assertEqual(2, Struct::staticVariable, "CPP: 2 == staticVariable");
	assertEqual(2, Struct::getStaticVariable(), "CPP: 2 == getStaticVariable()");
	Struct::setStaticVariable(3);
	assertEqual(3, Struct::staticVariable, "CPP: 3 == staticVariable");
	assertEqual(3, Struct::getStaticVariable(), "CPP: 3 == getStaticVariable()");
}
