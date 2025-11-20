// Test nested_variables for asmjs target
// REQUIRES: asmjs
// RUN: mkdir -p %t
// RUN: %cheerp_clang -O1 -frtti -I%S -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-asmjs -o %t/out %s
// RUN: cp %S/webworker-polyfill.js %t/ 2>/dev/null || true
// RUN: cat %t/out > %t/driver.js
// RUN: cat %S/import-polyfills.js >> %t/driver.js 2>/dev/null || true
// RUN: cat %S/nested_variables.testing.js >> %t/driver.js
// RUN: echo 'var EXPORTS = getExports()' >> %t/driver.js
// RUN: echo 'getPromise(EXPORTS).then(_=>{onInstantiation(getExports())}).catch(err => {onError(err)})' >> %t/driver.js
// RUN: (set -o pipefail; %node %t/driver.js 2>&1 | %FileCheck %s)
// CHECK: CPP: 1 == nestedVariable : SUCCESS
// CHECK: CPP: 1 == getNestedVariable() : SUCCESS
// CHECK: CPP: 2 == nestedVariable : SUCCESS
// CHECK: CPP: 2 == getNestedVariable() : SUCCESS
// CHECK: CPP: 3 == nestedVariable : SUCCESS
// CHECK: CPP: 3 == getNestedVariable() : SUCCESS
// CHECK-NOT: FAILURE

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

[[cheerp::jsexport]]
int main() {
	// Reset nested state to handle multiple runs
	Module::nestedVariable = 1;
	
	assertEqual(1, Module::nestedVariable, "CPP: 1 == nestedVariable");
	assertEqual(1, Module::getNestedVariable(), "CPP: 1 == getNestedVariable()");
	Module::nestedVariable = 2;
	assertEqual(2, Module::nestedVariable, "CPP: 2 == nestedVariable");
	assertEqual(2, Module::getNestedVariable(), "CPP: 2 == getNestedVariable()");
	Module::setNestedVariable(3);
	assertEqual(3, Module::nestedVariable, "CPP: 3 == nestedVariable");
	assertEqual(3, Module::getNestedVariable(), "CPP: 3 == getNestedVariable()");
}
