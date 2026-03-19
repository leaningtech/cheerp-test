//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// REQUIRES: wasm

// RUN: mkdir -p %t

// testing vanilla
// RUN: regular_only run_if_wasm compile_mode_wasm -pthread -o %t/vanilla %s 2>&1
// RUN: regular_only run_if_wasm python3 %S/../helpers/create_driver.py %t/vanilla %S/atomic1.testing.js %t/vanilla_driver.js --module=vanilla --pthread
// RUN: regular_only run_if_wasm %node %t/vanilla_driver.js 2>&1 | %FileCheck %s

// testing es6
// RUN: regular_only run_if_wasm compile_mode_wasm -pthread -cheerp-make-module=es6 -o %t/es6.mjs %s 2>&1
// RUN: regular_only run_if_wasm python3 %S/../helpers/create_driver.py %t/es6.mjs %S/atomic1.testing.js %t/es6_driver.mjs --module=es6 --pthread
// RUN: regular_only run_if_wasm %node %t/es6_driver.mjs 2>&1 | %FileCheck %s

// testing closure
// RUN: regular_only run_if_wasm compile_mode_wasm -pthread -cheerp-make-module=closure -o %t/closure %s 2>&1
// RUN: regular_only run_if_wasm python3 %S/../helpers/create_driver.py %t/closure %S/atomic1.testing.js %t/closure_driver.js --module=closure --pthread
// RUN: regular_only run_if_wasm %node %t/closure_driver.js 2>&1 | %FileCheck %s

// testing genericjs
// RUN: regular_only run_if_wasm compile_mode_wasm -pthread -cheerp-make-module=commonjs -o %t/commonjs %s 2>&1
// RUN: regular_only run_if_wasm python3 %S/../helpers/create_driver.py %t/commonjs %S/atomic1.testing.js %t/commonjs_driver.js --module=commonjs --pthread
// RUN: regular_only run_if_wasm %node %t/commonjs_driver.js 2>&1 | %FileCheck %s

#include <tests.h>
#include <atomic>

struct structWithAtomicMember
{
	std::atomic<int> atomicValue;
};

struct normalStruct
{
	int value;
};

int main()
{
	std::atomic<int> val = 4205;
	val.fetch_add(1391);
	val.fetch_and(6234);
	val.fetch_or(9196);
	val.fetch_sub(4985);
	val.fetch_xor(7578);
	assertPrint("Atomic binary operations:", val.load());
	// CHECK: Atomic binary operations: 15641

	int b = 90;
	b += val.exchange(3125);
	b += val.fetch_add(7264);
	b += val.fetch_sub(4981);
	b += val.fetch_or(2037);
	b += val.fetch_and(8185);
	b += val.load();
	assertPrint("Atomic binary return values:", b);
	// CHECK: Atomic binary return values: 46915

	structWithAtomicMember s1;
	s1.atomicValue.store(3);
	s1.atomicValue.fetch_add(95);
	s1.atomicValue.fetch_and(51);
	assertPrint("Atomic value inside struct:", s1.atomicValue.load());
	//CHECK: Atomic value inside struct: 34

	normalStruct s2 = {3};
	std::atomic<normalStruct> s3;
	s3.store(s2);
	s2.value = 25;
	s2 = s3.load();
	assertPrint("Atomic struct:", s2.value);
	// CHECK: Atomic struct: 3
}

