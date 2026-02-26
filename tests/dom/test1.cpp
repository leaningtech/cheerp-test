//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: js, regular

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j | %FileCheck %s

#include <tests.h>
#include <cheerp/client.h>

void webMain()
{
	//Test JS Arrays
	client::Array* ar = new client::Array(1,2,3);

	assertPrint("JS Array 1/2:", (int)*(*ar)[0]);
	// CHECK: JS Array 1/2: 1
	assertPrint("JS Array 2/2:", (int)*(*ar)[1]);
	// CHECK: JS Array 2/2: 2

	assertEqual(ar->indexOf(1), 0, "JS Array::indexOf 1/4:");
	// CHECK: JS Array::indexOf 1/4: SUCCESS
	assertEqual(ar->indexOf(4), -1, "JS Array::indexOf 2/4:");
	// CHECK: JS Array::indexOf 2/4: -1
	assertEqual(ar->indexOf(new client::Number(1)), -1, "JS Array::indexOf 3/4:");
	// CHECK: JS Array::indexOf 3/4: -1
	assertEqual(ar->indexOf(new client::Number(4)), -1, "JS Array::indexOf 4/4:");
	// CHECK: JS Array::indexOf 4/4: -1
	assertPrint("JS Array::join:", ar->join(","));
	// CHECK: JS Array::join: 1,2,3

	assertEqual(client::Array::isArray(ar), true, "JS Array::isArray 1/2:");
	// CHECK: JS Array::isArray 1/2: 1 
	assertEqual(client::Array::isArray(nullptr), false, "JS Array::isArray 2/2:");
	// CHECK: JS Array::isArray 2/2: SUCCESS
	assertEqual((int)*ar->reduce(cheerp::Callback([](int val, int current) { return val+current; })), 6, "JS Array::reduce:");
	// CHECK: JS Array::reduce: 6


	// Test typed array
	client::Int8Array* ar8 = new client::Int8Array(3);
	(*ar8)[0] = 0x7fffffff;
	(*ar8)[1] = 42;

	assertPrint("Typed array 1/3:", (int)(signed char)(*ar8)[0]);
	// CHECK: Typed array 1/3: -1
	assertPrint("Typed array 2/3:", (int)(signed char)(*ar8)[1]);
	// CHECK: Typed array 2/3: 42
	assertPrint("Typed array 3/3:", (int)(signed char)(*ar8)[2]);
	// CHECK: Typed array 3/3: 0

	// Test ArrayRef
	auto ref1 = cheerp::makeArrayRef(ar);
	assertEqual((int)*ref1[0], 1, "Array ref 1/6:");
	// CHECK: Array ref 1/6: 1
	assertEqual((int)*ref1[1], 2, "Array ref 2/6:");
	// CHECK: Array ref 2/6: 2
	const auto ref2 = cheerp::makeArrayRef(ar8);
	assertPrint("Array ref 3/6:", (int)(signed char)ref2[0]);
	// CHECK: Array ref 3/6: -1
	assertPrint("Array ref 4/6:", (int)(signed char)ref2[1]);
	// CHECK: Array ref 4/6: 42
	auto ref3 = cheerp::makeArrayRef(ar8);
	ref3[2] = 126;
	assertPrint("Array ref 5/6:", (int)(signed char)ref2[2]);
	// CHECK: Array ref 5/6: 126
	assertPrint("Array ref 6/6:", (int)(signed char)ref3[2]);
	// CHECK: Array ref 6/6: 126
	assertEqual(int(ref3->get_length()), 3, "Array ref operator->:");
	// CHECK: Array ref operator->: 3
}
