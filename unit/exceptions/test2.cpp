//===---------------------------------------------------------------------===//
//	Copyright 2021 Leaning Technlogies
//===----------------------------------------------------------------------===//


// REQUIRES: wasm

// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs -fexceptions %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -fexceptions %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: Non-ABI section can catch exceptions and rethrow 1/2 : SUCCESS
// CHECK: Non-ABI section can catch exceptions and rethrow 2/2 : SUCCESS

#include <tests.h>

[[cheerp::wasm]]
bool abiCatch = false;
[[cheerp::wasm]]
bool otherCatch = false;

[[cheerp::wasm]]
void doThrow()
{
	throw 1;
}

[[cheerp::genericjs]]
void middle()
{
	try {
		doThrow();
	} catch(int i) {
		otherCatch = true;
		throw;
	}
}

[[cheerp::wasm]]
void doCatch()
{
	try {
		middle();
	} catch(int i) {
		abiCatch = true;
	}
}
[[cheerp::wasm]]
void webMain()
{
	doCatch();
	assertEqual(otherCatch, true, "Non-ABI section can catch exceptions and rethrow 1/2");
	assertEqual(abiCatch, true, "Non-ABI section can catch exceptions and rethrow 2/2");
}
