//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 %then_run_js | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_js compile_mode_js -o %t/j %s 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 | %FileCheck %s

// CHECK: 64-bit wide pointer offsets 1/3: f
// CHECK: 64-bit wide pointer offsets 2/3: h
// CHECK: 64-bit wide pointer offsets 3/3: d

#include <tests.h>

void webMain()
{
	const char* buf = "abcdefghi";
	const char* volatile a = buf+5;
	volatile long long off = 2;
	const char* volatile b = a + off;
	const char* volatile c = a - off;

	assertPrint("64-bit wide pointer offsets 1/3:", *a);
	assertPrint("64-bit wide pointer offsets 2/3:", *b);
	assertPrint("64-bit wide pointer offsets 3/3:", *c);
}
