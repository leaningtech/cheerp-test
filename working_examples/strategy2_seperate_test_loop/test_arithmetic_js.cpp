// Test test_arithmetic for js target
// REQUIRES: js
// RUN: mkdir -p %t
// RUN: %cheerp_clang -O1 -frtti -I%S -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp -o %t/out %s
// RUN: cp %S/webworker-polyfill.js %t/ 2>/dev/null || true
// RUN: cat %t/out > %t/driver.js
// RUN: cat %S/import-polyfills.js >> %t/driver.js 2>/dev/null || true
// RUN: cat %S/test_arithmetic.testing.js >> %t/driver.js
// RUN: echo 'var EXPORTS = getExports()' >> %t/driver.js
// RUN: echo 'getPromise(EXPORTS).then(_=>{onInstantiation(getExports())}).catch(err => {onError(err)})' >> %t/driver.js
// RUN: (set -o pipefail; %node %t/driver.js 2>&1 | %FileCheck %s)
// CHECK: Addition : SUCCESS
// CHECK: Subtraction : SUCCESS
// CHECK: Multiplication : SUCCESS
// CHECK: Division : SUCCESS
// CHECK-NOT: FAILURE

//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

#include <tests.h>

[[cheerp::jsexport]]
int main() {
	int addition = 8 + 12;
	int subtraction = 25 - 5;
	int multiplication = 4 * 3;
	int division = 24 / 6;
	
	assertEqual(addition, 20, "Addition");
	assertEqual(subtraction, 20, "Subtraction");
	assertEqual(multiplication, 12, "Multiplication");
	assertEqual(division, 4, "Division");
	
	return 0;
}
