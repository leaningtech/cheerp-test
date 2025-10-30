// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
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
