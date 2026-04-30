//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>

void webMain()
{
	//Test pointer arithmetic
	int a[6] = { 1, 2, 3, 4, 5, 6};

	int* volatile a1=&a[1];
	int* volatile a2=&a[5];
	assertPrint("Pointer subtraction:", a2 - a1);
	// CHECK: Pointer subtraction: 4

	int* volatile a3=a+2;
	assertPrint("Pointer subtraction:", a3 - a);
	// CHECK: Pointer subtraction: 2
}
