//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 %then_run_js | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_js compile_mode_js -o %t/j %s 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 | %FileCheck %s

// CHECK: Rewriting i64 in types and expressions 1/2: 0xf00000000000001
// CHECK: Rewriting i64 in types and expressions 2/2: 0xf00000000000001

#include <tests.h>

struct Foo {
	int i;
	long long j;
};

int i = 1;
Foo fg{1,(long long)&i+0x0f00000000000001-(long long)&i};

void webMain()
{
	assertPrint("Rewriting i64 in types and expressions 1/2:", Hex((unsigned long long)fg.j));
	assertPrint("Rewriting i64 in types and expressions 2/2:", Hex((unsigned long long)(long long)&fg + 0x0f00000000000001 - (long long)&fg));
}