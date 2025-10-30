// XFAIL: *
// NOTE: This test requires .testing.js harness infrastructure not yet implemented in lit



// REQUIRES: wasm

// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -pthread %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s

#include <tests.h>

int testGlobal = 0;

[[cheerp::jsexport]]
int doCalculation(int amount)
{
	return amount * testGlobal;
}

void webMain()
{
	testGlobal = 5;
}
