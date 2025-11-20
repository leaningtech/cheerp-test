// Test static variables with jsexport across all targets
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
