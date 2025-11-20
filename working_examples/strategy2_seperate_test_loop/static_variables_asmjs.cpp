// Test static_variables for asmjs target
// REQUIRES: asmjs
// RUN: mkdir -p %t
// RUN: %cheerp_clang -O1 -frtti -I%S -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-asmjs -o %t/out %s
// RUN: cp %S/webworker-polyfill.js %t/ 2>/dev/null || true
// RUN: cat %t/out > %t/driver.js
// RUN: cat %S/import-polyfills.js >> %t/driver.js 2>/dev/null || true
// RUN: cat %S/static_variables.testing.js >> %t/driver.js
// RUN: echo 'var EXPORTS = getExports()' >> %t/driver.js
// RUN: echo 'getPromise(EXPORTS).then(_=>{onInstantiation(getExports())}).catch(err => {onError(err)})' >> %t/driver.js
// RUN: (set -o pipefail; %node %t/driver.js 2>&1 | %FileCheck %s)
// CHECK: CPP: 1 == staticVariable : SUCCESS
// CHECK: CPP: 1 == getStaticVariable() : SUCCESS
// CHECK: CPP: 2 == staticVariable : SUCCESS
// CHECK: CPP: 2 == getStaticVariable() : SUCCESS
// CHECK: CPP: 3 == staticVariable : SUCCESS
// CHECK: CPP: 3 == getStaticVariable() : SUCCESS
// CHECK-NOT: FAILURE

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

[[cheerp::jsexport]]
int main() {
	// Reset static state to handle multiple runs
	Struct::staticVariable = 1;
	
	assertEqual(1, Struct::staticVariable, "CPP: 1 == staticVariable");
	assertEqual(1, Struct::getStaticVariable(), "CPP: 1 == getStaticVariable()");
	Struct::staticVariable = 2;
	assertEqual(2, Struct::staticVariable, "CPP: 2 == staticVariable");
	assertEqual(2, Struct::getStaticVariable(), "CPP: 2 == getStaticVariable()");
	Struct::setStaticVariable(3);
	assertEqual(3, Struct::staticVariable, "CPP: 3 == staticVariable");
	assertEqual(3, Struct::getStaticVariable(), "CPP: 3 == getStaticVariable()");
}
