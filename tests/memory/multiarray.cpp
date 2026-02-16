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
	assertPrint("Multiarray support 1/4:", aTotal);
	// CHECK: Multiarray support 1/4: 66
	volatile int(*b)[4] = new int[3][4];
	for(int i=0;i<12;i++)
		b[i / 4][i % 4] = i;
	int bTotal = 0;
	for(int i=0;i<12;i++)
		bTotal += b[i / 4][i % 4];
	assertPrint("Multiarray support 2/4:", bTotal);
	// CHECK: Multiarray support 2/4: 66
	printf("test B: %d", bTotal);
	A c[3][4];
	assertPrint("Multiarray support 3/4:", int(A::constructionCount));
	// CHECK: Multiarray support 3/4: 12
	printf("test C: %d", int(A::constructionCount));
	A (*d)[4] = new A[3][4];
	assertPrint("Multiarray support 4/4:", int(A::constructionCount));
	// CHECK: Multiarray support 4/4: 24
	printf("test D: %d", int(A::constructionCount));
}

