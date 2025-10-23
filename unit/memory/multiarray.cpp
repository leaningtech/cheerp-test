// RUN: %cheerp_clang -O1 -frtti -I%S/.. -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: Multiarray support 1/ : SUCCESS
// CHECK: Multiarray support 2/ : SUCCESS
// CHECK: Multiarray support 3/ : SUCCESS
// CHECK: Multiarray support 4/ : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2018 Leaning Technlogies
//===----------------------------------------------------------------------===//

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
