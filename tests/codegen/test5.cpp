//===---------------------------------------------------------------------===//
//      Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s

// CHECK: Memmove backward 1/4: 0
// CHECK: Memmove backward 2/4: 0
// CHECK: Memmove backward 3/4: 1
// CHECK: Memmove backward 4/4: 2

#include <tests.h>

void webMain()
{
	int v[] = {0,1,2,3};
	memmove(v + 1, v, 3*sizeof(int));

	assertPrint("Memmove backward 1/4:", v[0]);
	assertPrint("Memmove backward 2/4:", v[1]);
	assertPrint("Memmove backward 3/4:", v[2]);
	assertPrint("Memmove backward 4/4:", v[3]);

}
