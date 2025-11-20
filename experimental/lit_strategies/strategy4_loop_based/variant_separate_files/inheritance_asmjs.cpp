// Test inheritance for asmjs target
// REQUIRES: asmjs
// RUN: mkdir -p %t
// RUN: %cheerp_clang -O1 -frtti -I%S -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-asmjs -o %t/out %s
// RUN: cp %S/webworker-polyfill.js %t/ 2>/dev/null || true
// RUN: cat %t/out > %t/driver.js
// RUN: cat %S/import-polyfills.js >> %t/driver.js 2>/dev/null || true
// RUN: cat %S/inheritance.testing.js >> %t/driver.js
// RUN: echo 'var EXPORTS = getExports()' >> %t/driver.js
// RUN: echo 'getPromise(EXPORTS).then(_=>{onInstantiation(getExports())}).catch(err => {onError(err)})' >> %t/driver.js
// RUN: %node %t/driver.js 2>&1

//===---------------------------------------------------------------------===//
//	Copyright 2024 Leaning Technologies
//===----------------------------------------------------------------------===//

struct [[cheerp::jsexport]] Interface {
	virtual int pureVirtualTest() = 0;

	virtual ~Interface() {}
};

struct [[cheerp::jsexport]] Base : Interface {
	Base() {}

	int memberBaseTest() { return 1; }
	int memberShadowTest() { return 0; }
	virtual int virtualTest() { return 0; }
	int pureVirtualTest() override { return 0; }

	static int staticBaseTest() { return 1; }
	static int staticShadowTest() { return 0; }
};

struct [[cheerp::jsexport]] Derived : Base {
	Derived() {}

	int memberDerivedTest() { return 1; }
	int memberShadowTest() { return 1; }
	int virtualTest() override { return 1; }
	int pureVirtualTest() override { return 1; }
	
	static int staticDerivedTest() { return 1; }
	static int staticShadowTest() { return 1; }
};
