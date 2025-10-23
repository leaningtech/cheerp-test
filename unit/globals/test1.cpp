// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: Access to global array 1/6 : SUCCESS
// CHECK: Access to global array 2/6 : SUCCESS
// CHECK: Access to global array 3/6 : SUCCESS
// CHECK: Access to global array using pointer 1/6 : SUCCESS
// CHECK: Access to global array using pointer 2/6 : SUCCESS
// CHECK: Access to global array using pointer 3/6 : SUCCESS
// CHECK: Access to global array using pointer with offset 1/6 : SUCCESS
// CHECK: Access to global array using pointer with offset 2/6 : SUCCESS
// CHECK: Access to global array using pointer with offset 3/6 : SUCCESS
// CHECK: Access to global array using local pointer with offset 1/3 : SUCCESS
// CHECK: Access to global array using local pointer with offset 2/3 : SUCCESS
// CHECK: Access to global array using local pointer with offset 3/3 : SUCCESS
// CHECK: Access to global array using local pointer with offset 1/6 : SUCCESS
// CHECK: Access to global array using local pointer with offset 2/6 : SUCCESS
// CHECK: Access to global array using local pointer with offset 3/6 : SUCCESS
// CHECK: Access to global array using local pointer with offset 4/6 : SUCCESS
// CHECK: Access to global array using local pointer with offset 5/6 : SUCCESS
// CHECK: Access to global array using local pointer with offset 6/6 : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2013-2014 Leaning Technlogies
//===----------------------------------------------------------------------===//

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
	assertEqual(a1, 't', "Access to global array 1/6");
	assertEqual(b1, 't', "Access to global array 2/6");
	assertEqual(c1, 't', "Access to global array 3/6");
	short d1=*numbers;
	short e1=numbers[0];
	short f1=numbers[3];
	assertEqual<short>(d1, 2, "Access to global array 4/6");
	assertEqual<short>(e1, 2, "Access to global array 5/6");
	assertEqual<short>(f1, 42, "Access to global array 6/6");

	char a2=*str2;
	char b2=str2[0];
	char c2=str2[1];
	assertEqual(a2, 't', "Access to global array using pointer 1/6");
	assertEqual(b2, 't', "Access to global array using pointer 2/6");
	assertEqual(c2, 'e', "Access to global array using pointer 3/6");
	short d2=*numbers2;
	short e2=numbers2[0];
	short f2=numbers2[3];
	assertEqual<short>(d2, 2, "Access to global array using pointer 4/6");
	assertEqual<short>(e2, 2, "Access to global array using pointer 5/6");
	assertEqual<short>(f2, 42, "Access to global array using pointer 6/6");

	char a3=*str3;
	char b3=str3[0];
	char c3=str3[1];
	assertEqual(a3, 's', "Access to global array using pointer with offset 1/6");
	assertEqual(b3, 's', "Access to global array using pointer with offset 2/6");
	assertEqual(c3, 't', "Access to global array using pointer with offset 3/6");
	short d3=*numbers3;
	short e3=numbers3[0];
	short f3=numbers3[3];
	assertEqual<short>(d3, 7, "Access to global array using pointer with offset 4/6");
	assertEqual<short>(e3, 7, "Access to global array using pointer with offset 5/6");
	assertEqual<short>(f3, 8, "Access to global array using pointer with offset 6/6");

	const char* str4 = str+2;
	char a4=*str4;
	char b4=str4[0];
	char c4=str4[1];
	assertEqual(a4, 's', "Access to global array using local pointer with offset 1/3");
	assertEqual(b4, 's', "Access to global array using local pointer with offset 2/3");
	assertEqual(c4, 't', "Access to global array using local pointer with offset 3/3");

	char** lstr6 = str5;
	char a5=*lstr6[0];
	char b5=lstr6[0][0];
	char c5=lstr6[0][1];
	assertEqual(a5, 't', "Access to global array using local pointer with offset 1/6");
	assertEqual(b5, 't', "Access to global array using local pointer with offset 2/6");
	assertEqual(c5, 'e', "Access to global array using local pointer with offset 3/6");

	char** lstr7 = str6;
	char a6=*lstr7[0];
	char b6=lstr7[0][0];
	char c6=lstr7[0][1];
	assertEqual(a6, 't', "Access to global array using local pointer with offset 4/6");
	assertEqual(b6, 't', "Access to global array using local pointer with offset 5/6");
	assertEqual(c6, 'S', "Access to global array using local pointer with offset 6/6");
}
