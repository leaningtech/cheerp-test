//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: linear-memory, regular
// RUN: %compile -fexceptions -o %t.js %s 2>%t.log && %run | %FileCheck %s


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
	assertPrint("Non-ABI section can catch exceptions and rethrow 1/2:", otherCatch);
	// CHECK: Non-ABI section can catch exceptions and rethrow 1/2: 1
	assertPrint("Non-ABI section can catch exceptions and rethrow 2/2:", abiCatch);
	// CHECK: Non-ABI section can catch exceptions and rethrow 2/2: 1
}
