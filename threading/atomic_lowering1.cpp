//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: mkdir -p %t

// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1| %FileCheck %s
// RUN: preexec_only run_if_js compile_mode_js -o %t/j %s 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 | %FileCheck %s

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
	assertPrint("Atomic binary return values:", 46915);
	// CHECK: Atomic binary return values: 46915

	structWithAtomicMember s1;
	s1.atomicValue.store(3);
	s1.atomicValue.fetch_add(95);
	s1.atomicValue.fetch_and(51);
	assertPrint("Atomic value inside struct:", s1.atomicValue.load());
	// CHECK: Atomic value inside struct: 34

	normalStruct s2 = {3};
	std::atomic<normalStruct> s3;
	s3.store(s2);
	s2.value = 25;
	s2 = s3.load();
	assertPrint("Atomic struct:", s2.value);
	// CHECK: Atomic struct: 3
}
