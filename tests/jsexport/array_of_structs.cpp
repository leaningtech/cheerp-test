//===---------------------------------------------------------------------===//
//	Copyright 2022 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t
// REQUIRES: regular

// Test vanilla module
// RUN: regular_only run_if_js compile_mode_js -o %t/vanilla.js %s 2>&1
// RUN: regular_only run_if_js python3 %S/../helpers/create_driver.py %t/vanilla.js %S/array_of_structs.testing.js %t/vanilla_driver.js --module=vanilla
// RUN: regular_only run_if_js %node %t/vanilla_driver.js 2>&1 | %FileCheck %s --check-prefixes=CHECK,CHECK-REGULAR

// Test ES6 module
// RUN: regular_only run_if_js compile_mode_js -cheerp-make-module=es6 -o %t/es6.mjs %s 2>&1
// RUN: regular_only run_if_js python3 %S/../helpers/create_driver.py %t/es6.mjs %S/array_of_structs.testing.js %t/es6_driver.mjs --module=es6
// RUN: regular_only run_if_js %node %t/es6_driver.mjs 2>&1 | %FileCheck %s --check-prefixes=CHECK,CHECK-REGULAR

// Test CommonJS module
// RUN: regular_only run_if_js compile_mode_js -cheerp-make-module=commonjs -o %t/commonjs.js %s 2>&1
// RUN: regular_only run_if_js python3 %S/../helpers/create_driver.py %t/commonjs.js %S/array_of_structs.testing.js %t/commonjs_driver.js --module=commonjs
// RUN: regular_only run_if_js %node %t/commonjs_driver.js 2>&1 | %FileCheck %s --check-prefixes=CHECK,CHECK-REGULAR

// Test Closure module
// RUN: regular_only run_if_js compile_mode_js -cheerp-make-module=closure -o %t/closure.js %s 2>&1
// RUN: regular_only run_if_js python3 %S/../helpers/create_driver.py %t/closure.js %S/array_of_structs.testing.js %t/closure_driver.js --module=closure
// RUN: regular_only run_if_js %node %t/closure_driver.js 2>&1 | %FileCheck %s --check-prefixes=CHECK,CHECK-REGULAR

// Also test wasm and asmjs with vanilla driver:
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1
// RUN: regular_only run_if_wasm python3 %S/../helpers/create_driver.py %t/w %S/array_of_structs.testing.js %t/w_driver.js --module=vanilla
// RUN: regular_only run_if_wasm %node %t/w_driver.js 2>&1 | %FileCheck %s

// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1
// RUN: regular_only run_if_asmjs python3 %S/../helpers/create_driver.py %t/a %S/array_of_structs.testing.js %t/a_driver.js --module=vanilla
// RUN: regular_only run_if_asmjs %node %t/a_driver.js 2>&1 | %FileCheck %s

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
			agnosticPrintString("getNext(i-th) == (i+1)-th");
	}
	if (getNext(&structs[N-1]) == &structs[N-1])
		agnosticPrintString("getNext(last) == last");

	// CHECK-REGULAR: getNext(i-th) == (i+1)-th
	// CHECK-REGULAR: getNext(i-th) == (i+1)-th
	// CHECK-REGULAR: getNext(i-th) == (i+1)-th
	// CHECK-REGULAR: getNext(i-th) == (i+1)-th
	// CHECK-REGULAR: getNext(i-th) == (i+1)-th
	// CHECK-REGULAR: getNext(i-th) == (i+1)-th 
	// CHECK-REGULAR: getNext(i-th) == (i+1)-th
	// CHECK-REGULAR: getNext(i-th) == (i+1)-th
	// CHECK-REGULAR: getNext(i-th) == (i+1)-th
	// CHECK-REGULAR: getNext(last) == last

	OrderedStruct* local = new OrderedStruct();
	for (int i=0; i<N; i++){
		if (getStruct(i) != local)
			agnosticPrintString("local != &struct[?]");
	}
	if (getNext(getNext(local)) == local)
		agnosticPrintString("getNext fixed point");

	// CHECK-REGULAR: local != &struct[?]
	// CHECK-REGULAR: local != &struct[?]
	// CHECK-REGULAR: local != &struct[?]
	// CHECK-REGULAR: local != &struct[?]
	// CHECK-REGULAR: local != &struct[?]
	// CHECK-REGULAR: local != &struct[?]
	// CHECK-REGULAR: local != &struct[?]
	// CHECK-REGULAR: local != &struct[?]
	// CHECK-REGULAR: local != &struct[?]
	// CHECK-REGULAR: local != &struct[?]
	// CHECK-REGULAR: getNext fixed point

	for (int i=0, j=0; i<N; i++, j=(j+7)%N)
		if (getStruct(i<j?i:j) == getSmaller(getStruct(i), getStruct(j)))
			agnosticPrintString("ordering");

	// CHECK-REGULAR: ordering
	// CHECK-REGULAR: ordering
	// CHECK-REGULAR: ordering
	// CHECK-REGULAR: ordering
	// CHECK-REGULAR: ordering
	// CHECK-REGULAR: ordering
	// CHECK-REGULAR: ordering
	// CHECK-REGULAR: ordering
	// CHECK-REGULAR: ordering
	// CHECK-REGULAR: ordering
	// CHECK-REGULAR: Custom has no next 1/1 SUCCESS
	// CHECK-NOT: unexpected exit : FAILURE

}
