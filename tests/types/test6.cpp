//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: mkdir -p %t

// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1| %FileCheck %s
// RUN: preexec_only run_if_js compile_mode_js -o %t/j %s 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 | %FileCheck %s

#include <tests.h>
#include <string.h>

union U
{
	int i;
	float f;
	char a[5];
};

void webMain()
{
	//Test union
	U u, u2;
	u.f = 1234.567;
	assertPrint("Access to unions 1/2:", Hex(u.i));
	// CHECK: Access to unions 1/2: 0x449a5225
	assertPrint("Access to unions 2/2:", Hex((unsigned short)u.a[2]));
	// CHECK: Access to unions 2/2: 0xff9a

	//Memory ops on unions
	memcpy(&u2, &u, sizeof(u));
	assertPrint("Memops on unions 1/2:", Hex(u2.i));
	// CHECK: Memops on unions 1/2: 0x449a5225
	memset(&u2, 0, sizeof(u));
	assertPrint("Memops on unions 2/2:", u2.i);
	// CHECK: Memops on unions 2/2: 0

	// Test arrays of union
	U au2[2];
	U* volatile au=au2;
	au[1].f = 1234.567;
	assertPrint("Access to arrays of unions 1/2:", Hex(au[1].i));
	// CHECK: Access to arrays of unions 1/2: 0x449a5225
	au[0].f = 1234.567;
	assertPrint("Access to arrays of unions 2/2:", Hex(au[0].i));
	// CHECK: Access to arrays of unions 2/2: 0x449a5225
}

