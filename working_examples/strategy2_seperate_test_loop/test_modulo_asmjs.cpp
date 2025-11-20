// Test test_modulo for asmjs target
// REQUIRES: asmjs
// RUN: mkdir -p %t
// RUN: %cheerp_clang -O1 -frtti -I%S -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-asmjs -o %t/out %s
// RUN: cp %S/webworker-polyfill.js %t/ 2>/dev/null || true
// RUN: cat %t/out > %t/driver.js
// RUN: cat %S/import-polyfills.js >> %t/driver.js 2>/dev/null || true
// RUN: cat %S/test_modulo.testing.js >> %t/driver.js
// RUN: echo 'var EXPORTS = getExports()' >> %t/driver.js
// RUN: echo 'getPromise(EXPORTS).then(_=>{onInstantiation(getExports())}).catch(err => {onError(err)})' >> %t/driver.js
// RUN: (set -o pipefail; %node %t/driver.js 2>&1 | %FileCheck %s)
// CHECK: Modulo : SUCCESS
// CHECK: Equal check : SUCCESS
// CHECK: Not equal check : SUCCESS
// CHECK-NOT: FAILURE

//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

#include <tests.h>

[[cheerp::jsexport]]
int main() {
	int modulo = 17 % 5;
	int is_equal = (20 == 20) ? 1 : 0;
	int is_not_equal = (10 != 20) ? 1 : 0;
	
	assertEqual(modulo, 2, "Modulo");
	assertEqual(is_equal, 1, "Equal check");
	assertEqual(is_not_equal, 1, "Not equal check");
	
	return 0;
}
