// Test cat for wasm target
// REQUIRES: wasm
// RUN: mkdir -p %t
// RUN: %cheerp_clang -O1 -frtti -I%S -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -o %t/out %s
// RUN: cp %S/webworker-polyfill.js %t/ 2>/dev/null || true
// RUN: cat %t/out > %t/driver.js
// RUN: cat %S/import-polyfills.js >> %t/driver.js 2>/dev/null || true
// RUN: cat %S/cat.testing.js >> %t/driver.js
// RUN: echo 'var EXPORTS = getExports()' >> %t/driver.js
// RUN: echo 'getPromise(EXPORTS).then(_=>{onInstantiation(getExports())}).catch(err => {onError(err)})' >> %t/driver.js
// RUN: %node %t/driver.js 2>&1

//===---------------------------------------------------------------------===//
//	Copyright 2022 Leaning Technologies
//===----------------------------------------------------------------------===//

#include <tests.h>

#include <cheerp/client.h>

class [[cheerp::jsexport]] Cat {
public:
	[[cheerp::genericjs]]
	Cat(client::String* name) : age(name->operator int()) {}
	int age;

	[[cheerp::genericjs]]
	~Cat() {
		client::console.log("Print something to console");
	}
	int i0() {
		return age * age;
	}
};
