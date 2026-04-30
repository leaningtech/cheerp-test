//===---------------------------------------------------------------------===//
//	Copyright 2022 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: regular

// Test vanilla module
// RUN: %compile -o %t-vanilla.js %s 2>&1
// RUN: python3 %helpers/create_driver.py %t-vanilla.js %S/array_of_structs.testing.js %t-vanilla_driver.js --module=vanilla
// RUN: %node %t-vanilla_driver.js 2>&1 | %FileCheck %s --check-prefixes=CHECK,CHECK-%target --allow-unused-prefixes

// Test ES6 module
// RUN: %compile -cheerp-make-module=es6 -o %t-es6.mjs %s 2>&1
// RUN: python3 %helpers/create_driver.py %t-es6.mjs %S/array_of_structs.testing.js %t-es6_driver.mjs --module=es6
// RUN: %node %t-es6_driver.mjs 2>&1 | %FileCheck %s --check-prefixes=CHECK,CHECK-%target --allow-unused-prefixes

// Test CommonJS module
// RUN: %compile -cheerp-make-module=commonjs -o %t-commonjs.js %s 2>&1
// RUN: python3 %helpers/create_driver.py %t-commonjs.js %S/array_of_structs.testing.js %t-commonjs_driver.js --module=commonjs
// RUN: %node %t-commonjs_driver.js 2>&1 | %FileCheck %s --check-prefixes=CHECK,CHECK-%target --allow-unused-prefixes

// Test Closure module
// RUN: %compile -cheerp-make-module=closure -o %t-closure.js %s 2>&1
// RUN: python3 %helpers/create_driver.py %t-closure.js %S/array_of_structs.testing.js %t-closure_driver.js --module=closure
// RUN: %node %t-closure_driver.js 2>&1 | %FileCheck %s --check-prefixes=CHECK,CHECK-%target --allow-unused-prefixes

#include <iostream>
#include <cheerp/client.h>
#include <cheerp/types.h>
#include "./../tests.h"

struct
[[cheerp::jsexport]]
OrderedStruct
{
	static int global_id;
	int id;
	int a;
	int b;
	int someUnusedArray[100];
	bool hasNext;
	OrderedStruct() {
		id = global_id++;
		a = id*id;
		b = id* a;
		hasNext = false;
	}
	void setHasNext()
	{
		hasNext = true;
	}
	int doComputation(int c)
	{
		return a*b*c;
	}
	static int valueOf()
	{
		return 53;
	}
};

int OrderedStruct::global_id = 0;

[[cheerp::jsexport]]
OrderedStruct* getSmaller(OrderedStruct* a, OrderedStruct* b)
{
	if (a < b)
		return a;
	return b;
}

const int N = 10;
OrderedStruct structs[10];

[[cheerp::jsexport]]
OrderedStruct* getStruct(int i)
{
	return &structs[i];
}

[[cheerp::jsexport]]
OrderedStruct* getNext(OrderedStruct* x)
{
	if (x->hasNext == false)
		return x;
	return x+1;
}

int main()
{
	for (int i=0; i<N-1; i++)
		structs[i].setHasNext();

	for (int i=0; i<N-1; i++){
		if (getNext(&structs[i]) == &structs[i+1])
			assertPrint("getNext(i-th) == (i+1)-th");
	}
	if (getNext(&structs[N-1]) == &structs[N-1])
		assertPrint("getNext(last) == last");

	// CHECK-js: getNext(i-th) == (i+1)-th
	// CHECK-js: getNext(i-th) == (i+1)-th
	// CHECK-js: getNext(i-th) == (i+1)-th
	// CHECK-js: getNext(i-th) == (i+1)-th
	// CHECK-js: getNext(i-th) == (i+1)-th
	// CHECK-js: getNext(i-th) == (i+1)-th 
	// CHECK-js: getNext(i-th) == (i+1)-th
	// CHECK-js: getNext(i-th) == (i+1)-th
	// CHECK-js: getNext(i-th) == (i+1)-th
	// CHECK-js: getNext(last) == last

	OrderedStruct* local = new OrderedStruct();
	for (int i=0; i<N; i++){
		if (getStruct(i) != local)
			assertPrint("local != &struct[?]");
	}
	if (getNext(getNext(local)) == local)
		assertPrint("getNext fixed point");

	// CHECK-js: local != &struct[?]
	// CHECK-js: local != &struct[?]
	// CHECK-js: local != &struct[?]
	// CHECK-js: local != &struct[?]
	// CHECK-js: local != &struct[?]
	// CHECK-js: local != &struct[?]
	// CHECK-js: local != &struct[?]
	// CHECK-js: local != &struct[?]
	// CHECK-js: local != &struct[?]
	// CHECK-js: local != &struct[?]
	// CHECK-js: getNext fixed point

	for (int i=0, j=0; i<N; i++, j=(j+7)%N)
		if (getStruct(i<j?i:j) == getSmaller(getStruct(i), getStruct(j)))
			assertPrint("ordering");

	// CHECK-js: ordering
	// CHECK-js: ordering
	// CHECK-js: ordering
	// CHECK-js: ordering
	// CHECK-js: ordering
	// CHECK-js: ordering
	// CHECK-js: ordering
	// CHECK-js: ordering
	// CHECK-js: ordering
	// CHECK-js: ordering
	// CHECK-js: Custom has no next 1/1 SUCCESS
	// CHECK-NOT: unexpected exit : FAILURE

}
