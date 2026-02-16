//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// This test is for the musl atomic primivites.
// The contents below are copied from a musl header file (/arch/cheerp/atomic_arch.h) and should be in sync with this header file.
// The unnecessary parts have been removed for readability.
// They have been tested on native and work as intended.

// REQUIRES: wasm

// RUN: mkdir -p %t

// testing vanilla
// RUN: regular_only run_if_wasm compile_mode_wasm -pthread -o %t/vanilla %s 2>&1
// RUN: regular_only run_if_wasm python3 %S/../helpers/create_driver.py %t/vanilla %S/atomic3.testing.js %t/vanilla_driver.js --module=vanilla --pthread
// RUN: regular_only run_if_wasm %node %t/vanilla_driver.js 2>&1 | %FileCheck %s

// testing es6
// RUN: regular_only run_if_wasm compile_mode_wasm -pthread -cheerp-make-module=es6 -o %t/es6.mjs %s 2>&1
// RUN: regular_only run_if_wasm python3 %S/../helpers/create_driver.py %t/es6.mjs %S/atomic3.testing.js %t/es6_driver.js --module=es6 --pthread
// RUN: regular_only run_if_wasm %node %t/es6_driver.js 2>&1 | %FileCheck %s

// testing closure
// RUN: regular_only run_if_wasm compile_mode_wasm -pthread -cheerp-make-module=closure -o %t/closure %s 2>&1
// RUN: regular_only run_if_wasm python3 %S/../helpers/create_driver.py %t/closure %S/atomic3.testing.js %t/closure_driver.js --module=closure --pthread
// RUN: regular_only run_if_wasm %node %t/closure_driver.js 2>&1 | %FileCheck %s

// testing genericjs
// RUN: regular_only run_if_wasm compile_mode_wasm -pthread -cheerp-make-module=commonjs -o %t/commonjs %s 2>&1
// RUN: regular_only run_if_wasm python3 %S/../helpers/create_driver.py %t/commonjs %S/atomic3.testing.js %t/commonjs_driver.js --module=commonjs --pthread
// RUN: regular_only run_if_wasm %node %t/commonjs_driver.js 2>&1 | %FileCheck %s

#include <tests.h>
#include <cstdint>

// This test is for the musl atomic primivites.
// The contents below are copied from a musl header file (/arch/cheerp/atomic_arch.h) and should be in sync with this header file.
// The unnecessary parts have been removed for readability.
// They have been tested on native and work as intended.

static inline int a_cas(volatile int *p, int t, int s)
{
	return __sync_val_compare_and_swap_4(p, t, s);
}

static inline int a_swap(volatile int *p, int v)
{
	return __sync_swap(p, v);
}

static inline int a_fetch_add(volatile int *p, int v)
{
	return __sync_fetch_and_add(p, v);
}

static inline void a_and(volatile int *p, int v)
{
	__sync_and_and_fetch(p, v);
}

static inline void a_or(volatile int *p, int v)
{
	__sync_or_and_fetch(p, v);
}

static inline void a_inc(volatile int *p)
{
	__sync_add_and_fetch(p, 1);
}

static inline void a_dec(volatile int *p)
{
	__sync_add_and_fetch(p, -1);
}

static inline void a_store(volatile int *p, int x)
{
	__atomic_store(p, &x, __ATOMIC_SEQ_CST);
}

static inline int a_ctz_64(uint64_t x)
{
	if (x == 0)
		return 64;
	return __builtin_ctzll(x);
}

static inline int a_ctz_32(uint32_t x)
{
	if (x == 0)
		return 32;
	return __builtin_ctzl(x);
}

static inline int a_clz_32(uint32_t x)
{
	return __builtin_clz(x);
}

// End of contents for the file, now the test.

int main()
{
	// a_cas
	int a = 5;
	assertPrint("Return value of a_cas when value is swapped:", a_cas(&a, 5, 2));
	// CHECK: Return value of a_cas when value is swapped: 5
	assertPrint("Value after the a_cas swapped:", a);
	// CHECK: Value after the a_cas swapped: 2

	assertPrint("Value after the a_cas swapped:", a);
	// CHECK: Value after the a_cas swapped: 2
	assertPrint("Return value of a_cas when value is not swapped:", a_cas(&a, 5, 1));
	// CHECK: Return value of a_cas when value is not swapped: 2
	assertPrint("Value after the a_cas did not swap:", a);
	// CHECK: Value after the a_cas did not swap: 2

	// a_swap
	assertPrint("Return value of a_swap:", a_swap(&a, 3));
	// CHECK: Return value of a_swap: 2
	assertPrint("Value after a_swap:", a);
	// CHECK: Value after a_swap: 3

	// a_fetch_add
	assertPrint("Return value of a_fetch_add:", a_fetch_add(&a, 3));
	// CHECK: Return value of a_fetch_add: 3
	assertPrint("Value after a_fetch_add:", a);
	// CHECK: Value after a_fetch_add: 6

	// a_and
	a_and(&a, 3);
	assertPrint("Value after a_and:", a);
	// CHECK: Value after a_and: 2

	// a_or
	a_or(&a, 5);
	assertPrint("Value after a_or:", a);
	// CHECK: Value after a_or: 7

	// a_inc
	a_inc(&a);
	assertPrint("Value after a_inc:", a);
	// CHECK: Value after a_inc: 8

	// a_dec
	a_dec(&a);
	assertPrint("Value after a_dec:", a);
	// CHECK: Value after a_dec: 7

	// a_store
	a_store(&a, 5);
	assertPrint("Value after a_store:", a);
	// CHECK: Value after a_store: 5

	// a_ctz_64
	a = 6;
	assertPrint("Return of a_ctz_64:", a_ctz_64(a));
	// CHECK: Return of a_ctz_64: 1

	// a_ctz_32
	a = 24;
	assertPrint("Return of a_ctz_32:", a_ctz_32(a));
	// CHECK: Return of a_ctz_32: 3

	// a_clz_32
	assertPrint("Return of a_clz_32:", a_clz_32(a));
	// CHECK: Return of a_clz_32: 27

	return 0;
}
