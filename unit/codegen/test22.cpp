// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: Runtime based division and modulo 1/N : SUCCESS
// CHECK: Runtime based division and modulo 2/N : SUCCESS
// CHECK: Runtime based division and modulo 3/N : SUCCESS
// CHECK: Runtime based division and modulo 4/N : SUCCESS
// CHECK: Runtime based division and modulo 5/N : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2015 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>

extern "C"
{
long long __divti3 (unsigned long long numerator, unsigned long long denominator);
long long __modti3 (unsigned long long numerator, unsigned long long denominator);
long long __udivti3 (unsigned long long numerator, unsigned long long denominator);
long long __umodti3 (unsigned long long numerator, unsigned long long denominator);
}

void webMain()
{
	// Test runtime based 64-bit division and module
	unsigned long long a = 1LL << 32;
	unsigned long long b = 1000000000;
	long long udiv = __udivti3(a, b);
	assertEqual(udiv, 4LL, "Runtime based division and modulo 1/N");
	long long umod = __umodti3(a, b);
	assertEqual(umod, 294967296LL, "Runtime based division and modulo 2/N");
	long long c = -a;
	long long sdiv = __divti3(c, b);
	assertEqual((long)sdiv, -4L, "Runtime based division and modulo 3/N");
	long long smod = __modti3(c, b);
	assertEqual((long)smod, -294967296L, "Runtime based division and modulo 4/N");
	long long d = -1LL;
	long long udiv2 = __udivti3(d, d);
	assertEqual(udiv2, 1LL, "Runtime based division and modulo 5/N");
}
