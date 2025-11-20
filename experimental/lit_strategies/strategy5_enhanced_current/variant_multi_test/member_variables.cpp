// Test member variables with jsexport across all targets
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
// CHECK: CPP: 1 == memberVariable : SUCCESS
// CHECK: CPP: 1 == getMemberVariable() : SUCCESS
// CHECK: CPP: 2 == memberVariable : SUCCESS
// CHECK: CPP: 2 == getMemberVariable() : SUCCESS
// CHECK: CPP: 3 == memberVariable : SUCCESS
// CHECK: CPP: 3 == getMemberVariable() : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2024 Leaning Technologies
//===----------------------------------------------------------------------===//

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
	assertEqual(1, instance->memberVariable, "CPP: 1 == memberVariable");
	assertEqual(1, instance->getMemberVariable(), "CPP: 1 == getMemberVariable()");
	instance->memberVariable = 2;
	assertEqual(2, instance->memberVariable, "CPP: 2 == memberVariable");
	assertEqual(2, instance->getMemberVariable(), "CPP: 2 == getMemberVariable()");
	instance->setMemberVariable(3);
	assertEqual(3, instance->memberVariable, "CPP: 3 == memberVariable");
	assertEqual(3, instance->getMemberVariable(), "CPP: 3 == getMemberVariable()");
}
