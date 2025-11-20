// Test member_variables for asmjs target
// REQUIRES: asmjs
// RUN: mkdir -p %t
// RUN: %cheerp_clang -O1 -frtti -I%S -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-asmjs -o %t/out %s
// RUN: cp %S/webworker-polyfill.js %t/ 2>/dev/null || true
// RUN: cat %t/out > %t/driver.js
// RUN: cat %S/import-polyfills.js >> %t/driver.js 2>/dev/null || true
// RUN: cat %S/member_variables.testing.js >> %t/driver.js
// RUN: echo 'var EXPORTS = getExports()' >> %t/driver.js
// RUN: echo 'getPromise(EXPORTS).then(_=>{onInstantiation(getExports())}).catch(err => {onError(err)})' >> %t/driver.js
// RUN: (set -o pipefail; %node %t/driver.js 2>&1 | %FileCheck %s)
// CHECK: CPP: 1 == memberVariable : SUCCESS
// CHECK: CPP: 1 == getMemberVariable() : SUCCESS
// CHECK: CPP: 2 == memberVariable : SUCCESS
// CHECK: CPP: 2 == getMemberVariable() : SUCCESS
// CHECK: CPP: 3 == memberVariable : SUCCESS
// CHECK: CPP: 3 == getMemberVariable() : SUCCESS
// CHECK-NOT: FAILURE

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

[[cheerp::jsexport]]
int main() {
	// Reset member state to handle multiple runs
	instance->memberVariable = 1;
	
	assertEqual(1, instance->memberVariable, "CPP: 1 == memberVariable");
	assertEqual(1, instance->getMemberVariable(), "CPP: 1 == getMemberVariable()");
	instance->memberVariable = 2;
	assertEqual(2, instance->memberVariable, "CPP: 2 == memberVariable");
	assertEqual(2, instance->getMemberVariable(), "CPP: 2 == getMemberVariable()");
	instance->setMemberVariable(3);
	assertEqual(3, instance->memberVariable, "CPP: 3 == memberVariable");
	assertEqual(3, instance->getMemberVariable(), "CPP: 3 == getMemberVariable()");
}
