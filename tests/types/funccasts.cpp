//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// REQUIRES: linear-memory, regular
// RUN: %compile -cheerp-fix-wrong-func-casts -o %t.js %s 2>%t.log && %run | %FileCheck %s



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
