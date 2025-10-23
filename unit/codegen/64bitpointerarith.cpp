// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: 64-bit wide pointer offsets 1/3 : SUCCESS
// CHECK: 64-bit wide pointer offsets 2/3 : SUCCESS
// CHECK: 64-bit wide pointer offsets 3/3 : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2016 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>

void webMain()
{
	const char* buf = "abcdefghi";
	const char* volatile a = buf+5;
	volatile long long off = 2;
	const char* volatile b = a + off;
	const char* volatile c = a - off;
	assertEqual(*a, 'f', "64-bit wide pointer offsets 1/3");
	assertEqual(*b, 'h', "64-bit wide pointer offsets 2/3");
	assertEqual(*c, 'd', "64-bit wide pointer offsets 3/3");
}
