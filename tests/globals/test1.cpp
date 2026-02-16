//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t

// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1| %FileCheck %s
// RUN: preexec_only run_if_js compile_mode_js -o %t/j %s 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 | %FileCheck %s

#include <tests.h>

static const char str[]="testStr";

static const char* str2=str;

static const char* str3=str+2;

static char str4[] = "testStr3";

static char* str5[] = { str4 };

static char* str6[] = { &str4[3] };

static const short numbers[]= { 2, 0, 7, 42, 100, 8 };

static const short* numbers2=numbers;

static const short* numbers3=numbers+2;

void webMain()
{
	char a1=*str;
	char b1=str[0];
	char c1=str[3];
	assertPrint("Access to global array 1/6: ", a1);
	assertPrint("Access to global array 2/6: ", b1);
	assertPrint("Access to global array 3/6: ", c1);
	// CHECK: Access to global array 1/6: t
	// CHECK: Access to global array 2/6: t
	// CHECK: Access to global array 3/6: t

	short d1=*numbers;
	short e1=numbers[0];
	short f1=numbers[3];
	assertPrint<short>("Access to global array 4/6: ", d1);
	assertPrint<short>("Access to global array 5/6: ", e1);
	assertPrint<short>("Access to global array 6/6: ", f1);
	// CHECK: Access to global array 4/6: 2
	// CHECK: Access to global array 5/6: 2
	// CHECK: Access to global array 6/6: 42

	char a2=*str2;
	char b2=str2[0];
	char c2=str2[1];
	assertPrint("Access to global array using pointer 1/6:", a2);
	assertPrint("Access to global array using pointer 2/6:", b2);
	assertPrint("Access to global array using pointer 3/6:", c2);
	// CHECK: Access to global array using pointer 1/6: t
	// CHECK: Access to global array using pointer 2/6: t
	// CHECK: Access to global array using pointer 3/6: e

	short d2=*numbers2;
	short e2=numbers2[0];
	short f2=numbers2[3];
	assertPrint("Access to global array using pointer 4/6:", d2);
	assertPrint("Access to global array using pointer 5/6:", e2);
	assertPrint("Access to global array using pointer 6/6:", f2);
	// CHECK: Access to global array using pointer 4/6: 2
	// CHECK: Access to global array using pointer 5/6: 2
	// CHECK: Access to global array using pointer 6/6: 42

	char a3=*str3;
	char b3=str3[0];
	char c3=str3[1];
	assertPrint("Access to global array using pointer with offset 1/6:", a3);
	assertPrint("Access to global array using pointer with offset 2/6:", b3);
	assertPrint("Access to global array using pointer with offset 3/6:", c3);
	// CHECK: Access to global array using pointer with offset 1/6: s
	// CHECK: Access to global array using pointer with offset 2/6: s
	// CHECK: Access to global array using pointer with offset 3/6: t

	short d3=*numbers3;
	short e3=numbers3[0];
	short f3=numbers3[3];
	assertPrint("Access to global array using pointer with offset 4/6:", d3);
	assertPrint("Access to global array using pointer with offset 5/6:", e3);
	assertPrint("Access to global array using pointer with offset 6/6:", f3);
	// CHECK: Access to global array using pointer with offset 4/6: 7
	// CHECK: Access to global array using pointer with offset 5/6: 7
	// CHECK: Access to global array using pointer with offset 6/6: 8

	const char* str4 = str+2;
	char a4=*str4;
	char b4=str4[0];
	char c4=str4[1];
	assertPrint("Access to global array using local pointer with offset 1/3:", a4);
	assertPrint("Access to global array using local pointer with offset 2/3:", b4);
	assertPrint("Access to global array using local pointer with offset 3/3:", c4);
	// CHECK: Access to global array using local pointer with offset 1/3: s
	// CHECK: Access to global array using local pointer with offset 2/3: s
	// CHECK: Access to global array using local pointer with offset 3/3: t

	char** lstr6 = str5;
	char a5=*lstr6[0];
	char b5=lstr6[0][0];
	char c5=lstr6[0][1];
	assertPrint("Access to global array using local pointer with offset 1/6:", a5);
	assertPrint("Access to global array using local pointer with offset 2/6:", b5);
	assertPrint("Access to global array using local pointer with offset 3/6:", c5);
	// CHECK: Access to global array using local pointer with offset 1/6: t
	// CHECK: Access to global array using local pointer with offset 2/6: t
	// CHECK: Access to global array using local pointer with offset 3/6: e

	char** lstr7 = str6;
	char a6=*lstr7[0];
	char b6=lstr7[0][0];
	char c6=lstr7[0][1];
	assertPrint("Access to global array using local pointer with offset 4/6:", a6);
	assertPrint("Access to global array using local pointer with offset 5/6:", b6);
	assertPrint("Access to global array using local pointer with offset 6/6:", c6);
	// CHECK: Access to global array using local pointer with offset 4/6: t
	// CHECK: Access to global array using local pointer with offset 5/6: t
	// CHECK: Access to global array using local pointer with offset 6/6: S
}
