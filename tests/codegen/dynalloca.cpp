//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>

void webMain()
{
	//Test dynamically sized allocas
	volatile int size = 10;
	int sum = 0;
	if (size > 4)
	{
		int arr[size];
		for (int i = 0; i < size; i++)
		{
			arr[i] = i;
		}
		for (int i = 0; i < size; i++)
		{
			sum += arr[i];
		}
	}

	assertPrint("Dynamic alloca support 1/1:", sum);
	//CHECK: Dynamic alloca support 1/1: 45
}
