//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


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
