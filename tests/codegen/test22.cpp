//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_js compile_mode_js -o %t/j %s 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 | %FileCheck %s

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
	assertPrint("Runtime based division and modulo 1/N:", udiv); //CHECK: Runtime based division and modulo 1/N: 4
	long long umod = __umodti3(a, b);
	assertPrint("Runtime based division and modulo 2/N:", umod); //CHECK: Runtime based division and modulo 2/N: 294967296
	long long c = -a;
	long long sdiv = __divti3(c, b);
	assertPrint("Runtime based division and modulo 3/N:", sdiv); //CHECK: Runtime based division and modulo 3/N: -4
	long long smod = __modti3(c, b);
	assertPrint("Runtime based division and modulo 4/N:", smod); //CHECK: Runtime based division and modulo 4/N: -294967296
	long long d = -1LL;
	long long udiv2 = __udivti3(d, d);
	assertPrint("Runtime based division and modulo 5/N:", udiv2); //CHECK: Runtime based division and modulo 5/N: 1
}
