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
#include <string.h>

class A
{
public:
	int a;
	int b;
	A(int x):a(x),b(x)
	{
	}
};

void webMain()
{
	//Test arrays of native types
	int a[] = {1,2,3,4,5,6,7,8};
	int b[] = {1,2,3,4,5,6,7,8};
	int c[] = {0,0,0,0,0,0,0,0};

	memmove(c,a,8*sizeof(int));
	assertPrint("Memmove, (native types) on isolated memory:", c[3]);
	// CHECK: Memmove, (native types) on isolated memory: 4

	memmove(a+2,a,6*sizeof(int));
	assertPrint("Memmove, (native types) destination after source:", a[3]);
	// CHECK: Memmove, (native types) destination after source: 2

	memmove(b,b+2,6*sizeof(int));
	assertPrint("Memmove, (native types) destination before source:", b[3]);
	// CHECK: Memmove, (native types) destination before source: 6

	//Test arrays of objects
	A oa[] = {9,10,11,12,13,14,15,16};
	A ob[] = {9,10,11,12,13,14,15,16};
	A oc[] = {0,0,0,0,0,0,0,0};

	memmove(oc,oa,8*sizeof(A));
	assertPrint("Memmove, (object types) on isolated memory:", oc[3].b);
	// CHECK: Memmove, (object types) on isolated memory: 12

	memmove(oa+2,oa,6*sizeof(A));
	assertPrint("Memmove, (object types) destination after source:", oa[3].b);
	// CHECK: Memmove, (object types) destination after source: 10

	memmove(ob,ob+2,6*sizeof(A));
	assertPrint("Memmove, (object types) destination before source:", ob[3].b);
	// CHECK: Memmove, (object types) destination before source: 14

	//Test sparse arrays of native types
	float sa[9] = {0.0, 1.0, 0.0, -1.0, -1.0, 0.0, 1.0, -1.0, 0.0};
	float* volatile psa = &sa[4];
	bool result = (*psa >= (-1.0 - 1e-6) && *psa <= (-1.0 + 1e-6));
	assertPrint("Accessing sparse arrays:", (int)result);
	// CHECK: Accessing sparse arrays: 1
}
