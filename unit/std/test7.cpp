// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: std::set 1/4 : SUCCESS
// CHECK: std::set 2/4 : SUCCESS
// CHECK: std::set 3/4 : SUCCESS
// CHECK: std::set ordering 4/4 : SUCCESS
// CHECK: std::multiset 1/4 : SUCCESS
// CHECK: std::multiset 2/4 : SUCCESS
// CHECK: std::multiset 3/4 : SUCCESS
// CHECK: std::multiset ordering 4/4 : SUCCESS
// CHECK: std::unordered_set 1/4 : SUCCESS
// CHECK: std::unordered_set 2/4 : SUCCESS
// CHECK: std::unordered_set 3/4 : SUCCESS
// CHECK: std::unordered_set ordering 4/4 : SUCCESS
// CHECK: std::unordered_set of pointers 1/4 : SUCCESS
// CHECK: std::unordered_set of pointers 2/4 : SUCCESS
// CHECK: std::unordered_set of pointers 3/4 : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2014 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>
#include <set>
#include <unordered_set>

void testSet()
{
	std::set<int> a;
	assertEqual(a.size(), 0u, "std::set 1/4");
	a.insert(1);
	assertEqual(a.size(), 1u, "std::set 2/4");
	a.insert(10);
	a.insert(99);
	a.insert(100);
	a.insert(100);
	a.insert(-2);
	assertEqual(a.size(), 5u, "std::set 3/4");
	int expected[] = {-2, 1, 10, 99, 100};
	std::set<int>::iterator it = a.begin();
	bool correctOrder = true;
	for(int i=0;i<a.size();i++)
	{
		if(*it != expected[i])
			correctOrder = false;
		++it;
	}
	assertEqual(correctOrder, true, "std::set ordering 4/4");
}

void testMultiSet()
{
	std::multiset<int> a;
	assertEqual(a.size(), 0u, "std::multiset 1/4");
	a.insert(1);
	assertEqual(a.size(), 1u, "std::multiset 2/4");
	a.insert(10);
	a.insert(99);
	a.insert(100);
	a.insert(100);
	a.insert(-2);
	assertEqual(a.size(), 6u, "std::multiset 3/4");
	int expected[] = {-2, 1, 10, 99, 100, 100};
	std::multiset<int>::iterator it = a.begin();
	bool correctOrder = true;
	for(int i=0;i<a.size();i++)
	{
		if(*it != expected[i])
			correctOrder = false;
		++it;
	}
	assertEqual(correctOrder, true, "std::multiset ordering 4/4");
}

void testUnorderedSet()
{
	std::unordered_set<int> a;
	assertEqual(a.size(), 0u, "std::unordered_set 1/4");
	a.insert(1);
	assertEqual(a.size(), 1u, "std::unordered_set 2/4");
	a.insert(10);
	a.insert(99);
	a.insert(100);
	a.insert(100);
	a.insert(-2);
	assertEqual(a.size(), 5u, "std::unordered_set 3/4");
	int expected[] = {-2, 99, 100, 10, 1};
	std::unordered_set<int>::iterator it = a.begin();
	bool correctOrder = true;
	for(int i=0;i<a.size();i++)
	{
		if(*it != expected[i])
			correctOrder = false;
		++it;
	}
	assertEqual(correctOrder, true, "std::unordered_set ordering 4/4");
}

void testUnorderedSetOfPointers()
{
	std::unordered_set<void*> a;
	assertEqual(a.size(), 0u, "std::unordered_set of pointers 1/4");
	a.insert(NULL);
	assertEqual(a.size(), 1u, "std::unordered_set of pointers 2/4");
	int i1,i2,i3,i4;
	a.insert(&i1);
	a.insert(&i2);
	a.insert(&i3);
	a.insert(&i3);
	a.insert(&i4);
	assertEqual(a.size(), 5u, "std::unordered_set of pointers 3/4");
}

void testPointerSet()
{
	//These report an error as sets of pointers are not supported
	//std::set<void*> a;
	//std::multiset<void*> m;
}

void webMain()
{
	testSet();
	testMultiSet();
	testUnorderedSet();
	testPointerSet();
	testUnorderedSetOfPointers();
}
