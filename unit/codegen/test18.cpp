// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: Scalar evolution for pointers 1/8 : SUCCESS
// CHECK: Scalar evolution for pointers 2/8 : SUCCESS
// CHECK: Scalar evolution for pointers 3/8 : SUCCESS
// CHECK: Scalar evolution for pointers 4/8 : SUCCESS
// CHECK: Scalar evolution for pointers 5/8 : SUCCESS
// CHECK: Scalar evolution for pointers 6/8 : SUCCESS
// CHECK: Scalar evolution for pointers 7/8 : SUCCESS
// CHECK: Scalar evolution for pointers 8/8 : SUCCESS

//===---------------------------------------------------------------------===//
//      Copyright 2015 Leaning Technlogies
//===----------------------------------------------------------------------===//

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
	assertEqual(count1,7,"Scalar evolution for pointers 1/8");
	assertEqual(count2,8,"Scalar evolution for pointers 2/8");
	assertEqual(count3,7,"Scalar evolution for pointers 3/8");
	assertEqual(count4,8,"Scalar evolution for pointers 4/8");
	assertEqual(count5,8,"Scalar evolution for pointers 5/8");
	assertEqual(count6,9,"Scalar evolution for pointers 6/8");
	assertEqual(count7,7,"Scalar evolution for pointers 7/8");
	assertEqual(count8,8,"Scalar evolution for pointers 8/8");
}
