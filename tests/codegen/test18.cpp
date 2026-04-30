//===----------------------------------------------------------------------===//
//      Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s

#include <tests.h>

void webMain()
{
	int* a = new int[10];
	int count1 = 0;
	for(int* i = a+1; i < a+8; i++)
		count1++;
	int count2 = 0;
	for(int* i = a+1; i <= a+8; i++)
		count2++;
	int count3 = 0;
	for(int* i = a+8; i > a+1; i--)
		count3++;
	int count4 = 0;
	for(int* i = a+8; i >= a+1; i--)
		count4++;
	int count5 = 0;
	for(int* i = a+8; i > a; i--)
		count5++;
	int count6 = 0;
	for(int* i = a+8; i >= a; i--)
		count6++;
	int count7 = 0;
	for(int* i = a+1; i != a+8; i++)
		count7++;
	int count8 = 0;
	for(int* i = a+8; i != a; i--)
		count8++;
	assertPrint("Scalar evolution for pointers 1/8:", count1); // CHECK: Scalar evolution for pointers 1/8: 7
	assertPrint("Scalar evolution for pointers 2/8:", count2); // CHECK: Scalar evolution for pointers 2/8: 8
	assertPrint("Scalar evolution for pointers 3/8:", count3); // CHECK: Scalar evolution for pointers 3/8: 7
	assertPrint("Scalar evolution for pointers 4/8:", count4); // CHECK: Scalar evolution for pointers 4/8: 8
	assertPrint("Scalar evolution for pointers 5/8:", count5); // CHECK: Scalar evolution for pointers 5/8: 8
	assertPrint("Scalar evolution for pointers 6/8:", count6); // CHECK: Scalar evolution for pointers 6/8: 9
	assertPrint("Scalar evolution for pointers 7/8:", count7); // CHECK: Scalar evolution for pointers 7/8: 7
}