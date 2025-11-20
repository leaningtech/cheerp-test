// Test array_of_structs for js target
// REQUIRES: js
// RUN: mkdir -p %t
// RUN: %cheerp_clang -O1 -frtti -I%S -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp -o %t/out %s
// RUN: cp %S/webworker-polyfill.js %t/ 2>/dev/null || true
// RUN: cat %t/out > %t/driver.js
// RUN: cat %S/import-polyfills.js >> %t/driver.js 2>/dev/null || true
// RUN: cat %S/array_of_structs.testing.js >> %t/driver.js
// RUN: echo 'var EXPORTS = getExports()' >> %t/driver.js
// RUN: echo 'getPromise(EXPORTS).then(_=>{onInstantiation(getExports())}).catch(err => {onError(err)})' >> %t/driver.js
// RUN: %node %t/driver.js 2>&1

//===---------------------------------------------------------------------===//
//	Copyright 2022 Leaning Technologies
//===----------------------------------------------------------------------===//

#include <tests.h>

#include <iostream>
#include <cheerp/client.h>

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

	for (int i=0; i<N-1; i++)
		assertEqual(getNext(&structs[i]), &structs[i+1], "getNext(i-th) == (i+1)-th");
	assertEqual(getNext(&structs[N-1]), &structs[N-1], "getNext(last) == last");

	OrderedStruct* local = new OrderedStruct();
	for (int i=0; i<N; i++)
		assertEqual(getStruct(i) != local, true, "local != &struct[?]");
	assertEqual(getNext(getNext(local)), local, "getNext fixed point");

	for (int i=0, j=0; i<N; i++, j=(j+7)%N)
		assertEqual(getStruct(i<j?i:j), getSmaller(getStruct(i), getStruct(j)), "ordering");
}
