//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// X-FAIL: *

// REQUIRES: linear-memory

// RUN: mkdir -p %t

// should fail for asmjs 

// RUN: regular_only run_if_wasm compile_mode_wasm -cheerp-fix-wrong-func-casts -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -cheerp-fix-wrong-func-casts -o %t/a %s 2>&1 && node %t/a 2>&1| %FileCheck %s

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
	assertPrint("FixFunctionCasts:", r);
	//CHECK: FixFunctionCasts: 10
}
