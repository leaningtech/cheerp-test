// XFAIL: *
// NOTE: This test requires .testing.js harness infrastructure not yet implemented in lit

//===---------------------------------------------------------------------===//
//   Copyright 2015 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: Memmove on signed and unsigned types : SUCCESS
// CHECK: Memcpy on signed and unsigned types : SUCCESS
// CHECK: Memmove on template types : SUCCESS
// CHECK: Memcpy on template types : SUCCESS

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
	assertEqual(b[3], 4, "Memmove on signed and unsigned types");

	memcpy(c, a, 8 * sizeof(int));
	assertEqual(c[3], 4, "Memcpy on signed and unsigned types");

	testTemplatedMemIntrinsics<int, unsigned>();
}

template <typename A, typename B>
void testTemplatedMemIntrinsics() {
	A a[] = {1,2,3,4,5,6,7,8};
	B b[] = {0,0,0,0,0,0,0,0};
	B c[] = {0,0,0,0,0,0,0,0};

	memmove(b, a, 8 * sizeof(A));
	assertEqual(b[3], 4, "Memmove on template types");

	memcpy(c, a, 8 * sizeof(A));
	assertEqual(c[3], 4, "Memcpy on template types");
}
