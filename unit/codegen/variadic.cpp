// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: Passing NULL to variadic call 1/2 : SUCCESS
// CHECK: Passing NULL to variadic call 2/2 : SUCCESS
// CHECK: Using va_copy in variadic call : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2017 Leaning Technlogies
//===----------------------------------------------------------------------===//

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
	assertEqual(r1, 10, "Passing NULL to variadic call 1/2");
	int r2 = variadicSum(NULL,NULL);
	assertEqual(r2, 0, "Passing NULL to variadic call 2/2");
	int r3 = variadicSumTwice(NULL, &a,&b,&c,&d, NULL);
	assertEqual(r3, 20, "Using va_copy in variadic call");
}
