// Test global variables with jsexport across all targets
// RUN: mkdir -p %t
// RUN: %compile_for{wasm} -O1 -o %t/w %s
// RUN: %compile_for{js} -O1 -o %t/j %s
// RUN: %compile_for{asmjs} -O1 -o %t/a %s
//
// RUN: %run_if{wasm} test -f %t/w && test -f %t/w.wasm
// RUN: %run_if{js} test -f %t/j
// RUN: %run_if{asmjs} test -f %t/a
//
// RUN: %run_if{wasm} %node %t/w 2>&1 | %FileCheck %s --check-prefix=CHECK
// RUN: %run_if{js} %node %t/j 2>&1 | %FileCheck %s --check-prefix=CHECK
// RUN: %run_if{asmjs} %node %t/a 2>&1 | %FileCheck %s --check-prefix=CHECK
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
