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
#include <set>
#include <unordered_set>

void testSet()
{
	std::set<int> a;
	assertPrint("std::set 1/4:", a.size());
	// CHECK: std::set 1/4: 0
	a.insert(1);
	assertPrint("std::set 2/4:", a.size());
	// CHECK: std::set 2/4: 1
	a.insert(10);
	a.insert(99);
	a.insert(100);
	a.insert(100);
	a.insert(-2);
	assertPrint("std::set 3/4:", a.size());
	// CHECK: std::set 3/4: 5
	int expected[] = {-2, 1, 10, 99, 100};
	std::set<int>::iterator it = a.begin();
	bool correctOrder = true;
	for(int i=0;i<a.size();i++)
	{
		if(*it != expected[i])
			correctOrder = false;
		++it;
	}
	assertPrint("std::set ordering 4/4:", correctOrder);
	// CHECK-NOT: std::set ordering 4/4: 0
}

void testMultiSet()
{
	std::multiset<int> a;
	assertPrint("std::multiset 1/4:", a.size());
	// CHECK: std::multiset 1/4: 0
	a.insert(1);
	assertPrint("std::multiset 2/4:", a.size());
	// CHECK: std::multiset 2/4: 1
	a.insert(10);
	a.insert(99);
	a.insert(100);
	a.insert(100);
	a.insert(-2);
	assertPrint("std::multiset 3/4:", a.size());
	int expected[] = {-2, 1, 10, 99, 100, 100};
	std::multiset<int>::iterator it = a.begin();
	bool correctOrder = true;
	for(int i=0;i<a.size();i++)
	{
		if(*it != expected[i])
			correctOrder = false;
		++it;
	}
	assertPrint("std::multiset ordering 4/4:", correctOrder);
	// CHECK-NOT: std::multiset ordering 4/4: 0
}

void testUnorderedSet()
{
	std::unordered_set<int> a;
	assertPrint("std::unordered_set 1/4:", a.size());
	// CHECK: std::unordered_set 1/4: 0
	a.insert(1);
	assertPrint("std::unordered_set 2/4:", a.size());
	// CHECK: std::unordered_set 2/4: 1
	a.insert(10);
	a.insert(99);
	a.insert(100);
	a.insert(100);
	a.insert(-2);
	assertPrint("std::unordered_set 3/4:", a.size());
	// CHECK: std::unordered_set 3/4: 5
	int expected[] = {-2, 99, 100, 10, 1};
	std::unordered_set<int>::iterator it = a.begin();
	bool correctOrder = true;
	for(int i=0;i<a.size();i++)
	{
		if(*it != expected[i])
			correctOrder = false;
		++it;
	}
	assertPrint("std::unordered_set ordering 4/4:", correctOrder);
	// CHECK-NOT: std::unordered_set ordering 4/4: 0
}

void testUnorderedSetOfPointers()
{
	std::unordered_set<void*> a;
	assertPrint("std::unordered_set of pointers 1/3:", a.size());
	// CHECK: std::unordered_set of pointers 1/3: 0
	a.insert(NULL);
	assertPrint("std::unordered_set of pointers 2/3:", a.size());
	// CHECK: std::unordered_set of pointers 2/3: 1
	int i1,i2,i3,i4;
	a.insert(&i1);
	a.insert(&i2);
	a.insert(&i3);
	a.insert(&i3);
	a.insert(&i4);
	assertPrint("std::unordered_set of pointers 3/3:", a.size());
	// CHECK: std::unordered_set of pointers 3/3: 5
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
