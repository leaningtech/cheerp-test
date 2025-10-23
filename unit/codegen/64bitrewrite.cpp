// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: Rewriting i64 in types and expressions 1/2 : SUCCESS
// CHECK: Rewriting i64 in types and expressions 2/2 : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2020 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>

struct Foo {
	int i;
	long long j;
};

int i = 1;
Foo fg{1,(long long)&i+0x0f00000000000001-(long long)&i};

void webMain()
{
	assertEqual(fg.j, 0x0f00000000000001, "Rewriting i64 in types and expressions 1/2");
	assertEqual((long long)&fg + 0x0f00000000000001 - (long long)&fg, 0x0f00000000000001, "Rewriting i64 in types and expressions 2/2");
}
