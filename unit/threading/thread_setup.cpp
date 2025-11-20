


// REQUIRES: wasm

// This test verifies pthread compilation - no runtime checks needed
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -pthread -cheerp-make-module=es6 %s -o %t.mjs
// RUN: test -f %t.mjs
// RUN: test -f %t.wasm


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
