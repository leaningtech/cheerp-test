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
#include <stdlib.h>

struct A
{
	int a;
	float b;
};

union BL
{
	int a;
	float b;
};

void webMain()
{
	//Test legacy C memory allocation
	int* volatile a=(int*)malloc(2*sizeof(int));
	a[1] = 42;
	assertPrint("Access malloc allocated memory:", a[1]);
	// CHECK: Access malloc allocated memory: 42

	//Test legacy C memory calloc
	int* volatile d=(int*)calloc(2,sizeof(int));
	d[1] = 45;
	assertPrint("Access calloc allocated memory 1/3:", d[1]);	free(d);
	// CHECK: Access calloc allocated memory 1/3: 45
	d=(int*)calloc(1,sizeof(int)*2);
	d[1] = 44;
	assertPrint("Access calloc allocated memory 2/3:", d[1]);
	// CHECK: Access calloc allocated memory 2/3: 44
	free(d);
	volatile int elementCount = 1;
	d=(int*)calloc(elementCount,sizeof(int)*2);
	d[1] = 43;
	assertPrint("Access calloc allocated memory 3/3:", d[1]);
	// CHECK: Access calloc allocated memory 3/3: 43
	free(d);

	//Test legacy C memory realloc
	a[0] = 46;
	int* volatile e=(int*)realloc(a, 1*sizeof(int));
	assertPrint("Access realloc-ed memory 1/6:", e[0]);
	// CHECK: Access realloc-ed memory 1/6: 46
	int* volatile f=(int*)realloc(e, 3*sizeof(int));
	f[1] = 47;
	assertPrint("Access realloc-ed memory 2/6:", f[1]);
	// CHECK: Access realloc-ed memory 2/6: 47
	free(f);
	A* volatile objA= (A*)malloc(2*sizeof(A));
	objA[0].a = 42;
	A* volatile objB = (A*)realloc(objA, 1*sizeof(A));
	assertPrint("Access realloc-ed memory 3/6:", objB[0].a);
	// CHECK: Access realloc-ed memory 3/6: 42
	A* volatile objC = (A*)realloc(objB, 3*sizeof(A));
	objC[2].a = 43;
	assertPrint("Access realloc-ed memory 4/6:", objC[2].a);
	// CHECK: Access realloc-ed memory 4/6: 43
	BL* volatile objBLA= (BL*)malloc(2*sizeof(BL));
	objBLA[0].b = 2.3;
	BL* volatile objBLB = (BL*)realloc(objBLA, 1*sizeof(BL));
	assertPrint("Access realloc-ed memory 5/6:", objBLB[0].a);
	// CHECK: Access realloc-ed memory 5/6: 1075000115
	BL* volatile objBLC = (BL*)realloc(objBLB, 3*sizeof(BL));
	objBLC[2].b = 2.4;
	assertPrint("Access realloc-ed memory 6/6:", objBLC[2].a);
	// CHECK: Access realloc-ed memory 6/6: 1075419546

	//realloc should work with NULL pointer and behave like malloc
	e=(int*)realloc((int*)NULL, 1*sizeof(int));
	e[0] = 46;
	assertPrint("Access realloc-ed from NULL memory 1/3:", e[0]);
	// CHECK: Access realloc-ed from NULL memory 1/3: 46
	objB = (A*)realloc((A*)NULL, 1*sizeof(A));
	objB[0].a = 42;
	assertPrint("Access realloc-ed from NULL memory 2/3:", objB[0].a);
	// CHECK: Access realloc-ed from NULL memory 2/3: 42
	objBLB = (BL*)realloc((BL*)NULL, 1*sizeof(BL));
	objBLB[0].b = 2.3;
	assertPrint("Access realloc-ed from NULL memory 3/3:", objBLB[0].a);
	// CHECK: Access realloc-ed from NULL memory 3/3: 1075000115

	//Test C++ memory allocation
	int* volatile b=new int[2];
	b[1]=43;
	assertPrint("Access new allocated memory:", b[1]);
	// CHECK: Access new allocated memory: 43
	delete[] b;

	//Test variable sized C++ memory alllocation
	volatile int n = 2;
	int* volatile c=new int[n];
	c[1]=44;
	assertPrint("Access variable sized new allocated memory:", c[1]);
	// CHECK: Access variable sized new allocated memory: 44
	delete[] c;
}
