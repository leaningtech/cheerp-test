
//===---------------------------------------------------------------------===//
//	Copyright 2019-2021 Leaning Technlogies
//===----------------------------------------------------------------------===//

// REQUIRES: js

// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// CHECK: JS Array 1/2 : SUCCESS
// CHECK: JS Array 2/2 : SUCCESS
// CHECK: JS Array::indexOf 1/4 : SUCCESS
// CHECK: JS Array::indexOf 3/4 : SUCCESS
// CHECK: JS Array::indexOf 3/4 : SUCCESS
// CHECK: JS Array::indexOf 4/4 : SUCCESS
// CHECK: JS Array::isArray 1/2 : SUCCESS
// CHECK: JS Array::isArray 2/2 : SUCCESS
// CHECK: Typed array 1/3 : SUCCESS
// CHECK: Typed array 2/3 : SUCCESS
// CHECK: Typed array 3/3 : SUCCESS
// CHECK: Array ref 1/6 : SUCCESS
// CHECK: Array ref 2/6 : SUCCESS
// CHECK: Array ref 3/6 : SUCCESS
// CHECK: Array ref 4/5 : SUCCESS
// CHECK: Array ref 5/6 : SUCCESS
// CHECK: Array ref 6/6 : SUCCESS
// CHECK: Array ref operator-> : SUCCESS

#include <tests.h>
#include <cheerp/client.h>

void webMain()
{
	//Test JS Arrays
	client::Array* ar = new client::Array(1,2,3);

	assertEqual((int)*(*ar)[0], 1, "JS Array 1/2");
	assertEqual((int)*(*ar)[1], 2, "JS Array 2/2");

	assertEqual(ar->indexOf(1), 0, "JS Array::indexOf 1/4");
	assertEqual(ar->indexOf(4), -1, "JS Array::indexOf 3/4");
	// Number object are compared for identity, so we won't find it
	assertEqual(ar->indexOf(new client::Number(1)), -1, "JS Array::indexOf 3/4");
	assertEqual(ar->indexOf(new client::Number(4)), -1, "JS Array::indexOf 4/4");

	assertEqual(ar->join(","), new client::String("1,2,3"), "JS Array::join");

	assertEqual(client::Array::isArray(ar), true, "JS Array::isArray 1/2");
	assertEqual(client::Array::isArray(nullptr), false, "JS Array::isArray 2/2");
	assertEqual((int)*ar->reduce(cheerp::Callback([](int val, int current) { return val+current; })), 6, "JS Array::reduce");

	// Test typed array
	client::Int8Array* ar8 = new client::Int8Array(3);
	(*ar8)[0] = 0x7fffffff;
	(*ar8)[1] = 42;

	assertEqual((*ar8)[0], char(-1), "Typed array 1/3");
	assertEqual((*ar8)[1], char(42), "Typed array 2/3");
	assertEqual((*ar8)[2], char(0), "Typed array 3/3");

	// Test ArrayRef
	auto ref1 = cheerp::makeArrayRef(ar);
	assertEqual((int)*ref1[0], 1, "Array ref 1/6");
	assertEqual((int)*ref1[1], 2, "Array ref 2/6");
	const auto ref2 = cheerp::makeArrayRef(ar8);
	assertEqual(ref2[0], char(-1), "Array ref 3/6");
	assertEqual(ref2[1], char(42), "Array ref 4/5");
	auto ref3 = cheerp::makeArrayRef(ar8);
	ref3[2] = 126;
	assertEqual(ref2[2], char(126), "Array ref 5/6");
	assertEqual(ref3[2], char(126), "Array ref 6/6");
	assertEqual(int(ref3->get_length()), 3, "Array ref operator->");
}
