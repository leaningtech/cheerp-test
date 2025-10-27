//===---------------------------------------------------------------------===//
//	Copyright 2013 Leaning Technlogies
//===----------------------------------------------------------------------===//

// RUN: %if_js %cheerp_clang -O1 -frtti -I%S/.. -target cheerp %s -o %t.js %endif
// RUN: %if_js %node %t.js 2>&1 | %FileCheck %s %endif
// RUN: %if_asmjs %cheerp_clang -O1 -frtti -I%S/.. -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js %endif
// RUN: %if_asmjs %node %t_asmjs.js 2>&1 | %FileCheck %s %endif
// RUN: %if_wasm %cheerp_clang -O1 -frtti -I%S/.. -target cheerp-wasm %s -o %t_wasm.js %endif
// RUN: %if_wasm %node %t_wasm.js 2>&1 | %FileCheck %s %endif

// CHECK: Memmove, (native types) on isolated memory : SUCCESS
// CHECK: Memmove, (native types) destination after source : SUCCESS
// CHECK: Memmove, (native types) destination before source : SUCCESS
// CHECK: Memmove, (object types) on isolated memory : SUCCESS
// CHECK: Memmove, (object types) destination after source : SUCCESS
// CHECK: Memmove, (object types) destination before source : SUCCESS

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
	assertEqual(c[3], 4, "Memmove, (native types) on isolated memory");

	memmove(a+2,a,6*sizeof(int));
	assertEqual(a[3], 2, "Memmove, (native types) destination after source");

	memmove(b,b+2,6*sizeof(int));
	assertEqual(b[3], 6, "Memmove, (native types) destination before source");

	//Test arrays of objects
	A oa[] = {9,10,11,12,13,14,15,16};
	A ob[] = {9,10,11,12,13,14,15,16};
	A oc[] = {0,0,0,0,0,0,0,0};

	memmove(oc,oa,8*sizeof(A));
	assertEqual(oc[3].b, 12, "Memmove, (object types) on isolated memory");

	memmove(oa+2,oa,6*sizeof(A));
	assertEqual(oa[3].b, 10, "Memmove, (object types) destination after source");

	memmove(ob,ob+2,6*sizeof(A));
	assertEqual(ob[3].b, 14, "Memmove, (object types) destination before source");

	//Test sparse arrays of native types
	float sa[9] = {0.0, 1.0, 0.0, -1.0, -1.0, 0.0, 1.0, -1.0, 0.0};
	float* volatile psa = &sa[4];
	assertEqual(*psa, -1.0, 1e-6, "Accessing sparse arrays");

}
