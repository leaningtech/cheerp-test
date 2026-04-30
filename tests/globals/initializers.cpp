//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>
#include <stdlib.h>

int* p1 = (int*) malloc(900);
int* p2 = (int*) calloc(1, 900);
int* p3 = (int*) realloc((int*) 0, 900);

int main() {
	assertPrint("expected p1 memory to be allocated:", (p1 != 0));
	// CHECK: expected p1 memory to be allocated: 
	// CHECK-NOT: 0
	assertPrint("expected p2 memory to be allocated:", (p2 != 0));
	// CHECK: expected p2 memory to be allocated:
	// CHECK-NOT: 0
	assertPrint("expected p3 memory to be allocated:", (p3 != 0));
	// CHECK: expected p3 memory to be allocated:
	// CHECK-NOT: 0
}
