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
	static int constructionCount;
	static int destructionCount;
	int a;
	A():a(42)
	{
		constructionCount++;
	}
	~A()
	{
		destructionCount++;
	}
};

int A::constructionCount = 0;
int A::destructionCount = 0;

void webMain()
{
	A* a = new A[10];
	delete[] a;
	assertPrint("Array new support 1/2:", A::constructionCount);
	// CHECK: Array new support 1/2: 10
	assertPrint("Array new support 2/2:", A::destructionCount);
	// CHECK: Array new support 2/2: 10
}
