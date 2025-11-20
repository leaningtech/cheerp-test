// Test global_variables for js target
// REQUIRES: js
// RUN: mkdir -p %t
// RUN: %cheerp_clang -O1 -frtti -I%S -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp -o %t/out %s
// RUN: cp %S/webworker-polyfill.js %t/ 2>/dev/null || true
// RUN: cat %t/out > %t/driver.js
// RUN: cat %S/import-polyfills.js >> %t/driver.js 2>/dev/null || true
// RUN: cat %S/global_variables.testing.js >> %t/driver.js
// RUN: echo 'var EXPORTS = getExports()' >> %t/driver.js
// RUN: echo 'getPromise(EXPORTS).then(_=>{onInstantiation(getExports())}).catch(err => {onError(err)})' >> %t/driver.js
// RUN: %node %t/driver.js 2>&1

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
