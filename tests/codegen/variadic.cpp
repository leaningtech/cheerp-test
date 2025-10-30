//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_js compile_mode_js -o %t/j %s 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 | %FileCheck %s

#include <stdarg.h>
#include <stdlib.h>
#include <tests.h>

int variadicSumImpl(va_list args)
{
	int ret = 0;
	while(char* i = va_arg(args, char*))
	{
		ret += *i;
	}
	return ret;
}

int variadicSum(int a, ...)
{
	int ret = 0;
	va_list args;
	va_start(args,a);
	ret = variadicSumImpl(args);
	va_end(args);
	return ret;
}
int variadicSumTwice(int a, ...)
{
	int ret = 0;
	va_list args;
	va_start(args,a);
	va_list copy;
	va_copy(copy,args);
	ret = variadicSumImpl(args);
	va_end(args);
	ret  += variadicSumImpl(copy);
	va_end(copy);
	return ret;
}
void webMain()
{
	int i = NULL;
	int *p = NULL;
	char a = 1;
	char b = 2;
	char c = 3;
	char d = 4;
	int r1 = variadicSum(NULL,&a,&b,&c,&d,NULL);
	assertPrint("Passing NULL  to variadic call 1/2:", r1);
	// CHECK: Passing NULL to variadic call 1/2: 10
	// assertEqual(r1, 10, "Passing NULL to variadic call 1/2");
	int r2 = variadicSum(NULL,NULL);
	assertPrint("Passing NULL  to variadic call 2/2:", r2);
	// CHECK: Passing NULL to variadic call 2/2: 0
	// assertEqual(r2, 0, "Passing NULL to variadic call 2/2");
	int r3 = variadicSumTwice(NULL, &a,&b,&c,&d, NULL);
	// assertEqual(r3, 20, "Using va_copy in variadic call");
	assertPrint("Using va_copy in variadic call:", r3);
	// CHECK: Using va_copy in variadic call: 20

}
