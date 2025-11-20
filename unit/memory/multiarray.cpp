//===---------------------------------------------------------------------===//
//	Copyright 2018 Leaning Technlogies
//===----------------------------------------------------------------------===//

// RUN: %if_js %cheerp_clang -O1 -frtti -I%S/.. -target cheerp %s -o %t.js %endif
// RUN: %if_js %node %t.js 2>&1 | %FileCheck %s %endif

// RUN: %if_asmjs %cheerp_clang -O1 -frtti -I%S/.. -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js %endif
// RUN: %if_asmjs %node %t_asmjs.js 2>&1 | %FileCheck %s %endif

// RUN: %if_wasm %cheerp_clang -O1 -frtti -I%S/.. -target cheerp-wasm %s -o %t_wasm.js %endif
// RUN: %if_wasm %node %t_wasm.js 2>&1 | %FileCheck %s --check %endif


#include <tests.h>

class A
{
public:
	static volatile int constructionCount;
	int a;
	A():a(42)
	{
		constructionCount++;
	}
};

volatile int A::constructionCount = 0;

void webMain()
{
	volatile int a[3][4];
	for(int i=0;i<12;i++)
		a[i / 4][i % 4] = i;
	int aTotal = 0;
	for(int i=0;i<12;i++)
		aTotal += a[i / 4][i % 4];
	assertEqual(aTotal, 66, "Multiarray support 1/");
	printf("test A: %d", aTotal);
	volatile int(*b)[4] = new int[3][4];
	for(int i=0;i<12;i++)
		b[i / 4][i % 4] = i;
	int bTotal = 0;
	for(int i=0;i<12;i++)
		bTotal += b[i / 4][i % 4];
	assertEqual(bTotal, 66, "Multiarray support 2/");
	printf("test B: %d", bTotal);
	A c[3][4];
	assertEqual(int(A::constructionCount), 12, "Multiarray support 3/");
	printf("test C: %d", int(A::constructionCount));
	A (*d)[4] = new A[3][4];
	assertEqual(int(A::constructionCount), 24, "Multiarray support 4/");
	printf("test D: %d", int(A::constructionCount));
}


// CHECK: Multiarray support 1/ : SUCCESS
// CHECK: Multiarray support 2/ : SUCCESS
// CHECK: Multiarray support 3/ : SUCCESS
// CHECK: Multiarray support 4/ : SUCCESS