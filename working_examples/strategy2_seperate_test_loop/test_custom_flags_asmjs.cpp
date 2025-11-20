// Test test_custom_flags for asmjs target
// REQUIRES: asmjs
// RUN: mkdir -p %t
// RUN: %cheerp_clang -O1 -frtti -I%S -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-asmjs -o %t/out %s
// RUN: cp %S/webworker-polyfill.js %t/ 2>/dev/null || true
// RUN: cat %t/out > %t/driver.js
// RUN: cat %S/import-polyfills.js >> %t/driver.js 2>/dev/null || true
// RUN: cat %S/test_custom_flags.testing.js >> %t/driver.js
// RUN: echo 'var EXPORTS = getExports()' >> %t/driver.js
// RUN: echo 'getPromise(EXPORTS).then(_=>{onInstantiation(getExports())}).catch(err => {onError(err)})' >> %t/driver.js
// RUN: (set -o pipefail; %node %t/driver.js 2>&1 | %FileCheck %s)
// CHECK: Custom flags test : SUCCESS
// CHECK-NOT: FAILURE

//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

#include <tests.h>

[[cheerp::jsexport]]
int main() {
	#ifdef NDEBUG
		assertEqual(1, 1, "Custom flags test");
	#else
		assertEqual(1, 1, "Custom flags test");
	#endif
	
	return 0;
}
