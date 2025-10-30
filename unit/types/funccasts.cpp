//===---------------------------------------------------------------------===//
//	Copyright 2019 Leaning Technlogies
//===----------------------------------------------------------------------===//

// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp -cheerp-fix-wrong-func-casts %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs -cheerp-fix-wrong-func-casts %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-fix-wrong-func-casts %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: FixFunctionCasts 1/1 : SUCCESS

#include <tests.h>

#include <cstdio>
#include <cstdlib>

int foo(int a, int b, int c, int d)
{
	return a+b+c+d;
}
int bar(int a, int b, int c, int d, int e)
{
	return a+b+c+d+e;
}

typedef int (*fptr)(int, int, int, int, int);
void webMain()
{
	fptr f = unitBlackBox(1) ? (fptr)foo : bar;
	int r = f(1,2,3,4,5);
	assertEqual(r, 10, "FixFunctionCasts 1/1");
}
