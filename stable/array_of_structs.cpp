//===---------------------------------------------------------------------===//
//	Copyright 2022 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t
// Preexecution mode (PREEXECUTE_MODE=true):
// RUN: preexec_only run_if_js %valgrind compile_mode_js -o %t/j_pre %s 2>&1 | %FileCheck %s 
// RUN: preexec_only run_if_asmjs %valgrind compile_mode_asmjs -o %t/a_pre %s 2>&1 | %FileCheck %s 
//
// Regular mode (PREEXECUTE_MODE=false):
// RUN: regular_only run_if_wasm %valgrind compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s 
// RUN: regular_only run_if_js %valgrind compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s 
// RUN: regular_only run_if_asmjs %valgrind compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1 | %FileCheck %s 

#include <iostream>
#include <cheerp/client.h>
#include <cheerp/types.h>
#include "tests.h"

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
			__preexecute_print_case("getNext(i-th) == (i+1)-th");
	}
	if (getNext(&structs[N-1]) == &structs[N-1])
		__preexecute_print_case("getNext(last) == last");

	// CHECK: getNext(i-th) == (i+1)-th
	// CHECK: getNext(i-th) == (i+1)-th
	// CHECK: getNext(i-th) == (i+1)-th
	// CHECK: getNext(i-th) == (i+1)-th
	// CHECK: getNext(i-th) == (i+1)-th
	// CHECK: getNext(i-th) == (i+1)-th 
	// CHECK: getNext(i-th) == (i+1)-th
	// CHECK: getNext(i-th) == (i+1)-th
	// CHECK: getNext(i-th) == (i+1)-th
	// CHECK: getNext(last) == last

	OrderedStruct* local = new OrderedStruct();
	for (int i=0; i<N; i++){
		if (getStruct(i) != local)
			__preexecute_print_case("local != &struct[?]");
	}
	if (getNext(getNext(local)) == local)
		__preexecute_print_case("getNext fixed point");

	// CHECK: local != &struct[?]
	// CHECK: local != &struct[?]
	// CHECK: local != &struct[?]
	// CHECK: local != &struct[?]
	// CHECK: local != &struct[?]
	// CHECK: local != &struct[?]
	// CHECK: local != &struct[?]
	// CHECK: local != &struct[?]
	// CHECK: local != &struct[?]
	// CHECK: local != &struct[?]
	// CHECK: getNext fixed point

	for (int i=0, j=0; i<N; i++, j=(j+7)%N)
		if (getStruct(i<j?i:j) == getSmaller(getStruct(i), getStruct(j)))
			__preexecute_print_case("ordering");

	// CHECK: ordering
	// CHECK: ordering
	// CHECK: ordering
	// CHECK: ordering
	// CHECK: ordering
	// CHECK: ordering
	// CHECK: ordering
	// CHECK: ordering
	// CHECK: ordering
	// CHECK: ordering
}
