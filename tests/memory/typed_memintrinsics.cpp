//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s




#include <tests.h>
#include <string.h>

template <typename A, typename B>
void testTemplatedMemIntrinsics();

void webMain()
{
	int a[] = {1,2,3,4,5,6,7,8};
	unsigned b[] = {0,0,0,0,0,0,0,0};
	unsigned c[] = {0,0,0,0,0,0,0,0};

	memmove(b, a, 8 * sizeof(int));
	assertPrint("Memmove on signed and unsigned types:", b[3]);
	// CHECK: Memmove on signed and unsigned types: 4


	memcpy(c, a, 8 * sizeof(int));
	assertPrint("Memcpy on signed and unsigned types:", c[3]);
	// CHECK: Memcpy on signed and unsigned types: 4

	testTemplatedMemIntrinsics<int, unsigned>();
}

template <typename A, typename B>
void testTemplatedMemIntrinsics() {
	A a[] = {1,2,3,4,5,6,7,8};
	B b[] = {0,0,0,0,0,0,0,0};
	B c[] = {0,0,0,0,0,0,0,0};

	memmove(b, a, 8 * sizeof(A));
	assertPrint("Memmove on template types:", b[3]);
	// CHECK: Memmove on template types: 4


	memcpy(c, a, 8 * sizeof(A));
	assertPrint("Memcpy on template types:", c[3]);
	// CHECK: Memcpy on template types: 4
}
