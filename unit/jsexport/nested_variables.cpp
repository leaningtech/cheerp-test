// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
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
