//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>

struct T
{
	float p[3];
	float* x;
	float* y;
	float* z;
};

extern T t;

T t2 = {4,5,6,&t.p[0],&t.p[1],&t.p[2]};

T t = {1,2,3,&t2.p[0],&t2.p[1],&t2.p[2]};

void webMain()
{
	assertPrint("Member references to other members 1/6:", *t.x);
	// CHECK: Member references to other members 1/6: 4.0000000000
	assertPrint("Member references to other members 2/6:", *t.y); 
	// CHECK: Member references to other members 2/6: 5.0000000000
	assertPrint("Member references to other members 3/6:", *t.z);
	// CHECK: Member references to other members 3/6: 6.0000000000

	assertPrint("Member references to other members 4/6:", *t2.x);
	// CHECK: Member references to other members 4/6: 1.0000000000
	assertPrint("Member references to other members 5/6:", *t2.y);
	// CHECK: Member references to other members 5/6: 2.0000000000
	assertPrint("Member references to other members 6/6:", *t2.z);
	// CHECK: Member references to other members 6/6: 3.0000000000
}
