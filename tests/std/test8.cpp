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
#include <map>
#include <unordered_map>

void testMap()
{
	std::map<int, int> a;
	assertPrint("std::map 1/4:", a.size());
	// CHECK: std::map 1/4: 0
	a.insert(std::make_pair(1,2));
	assertPrint("std::map 2/4:", a.size());
	// CHECK: std::map 2/4: 1
	a.insert(std::make_pair(10,11));
	a.insert(std::make_pair(99,100));
	a.insert(std::make_pair(100,101));
	a.insert(std::make_pair(100,101));
	a.insert(std::make_pair(-2, -1));
	assertPrint("std::map 3/4:", 5);
	// CHECK: std::map 3/4: 5
	int expected[] = {-2, 1, 10, 99, 100};
	std::map<int, int>::iterator it = a.begin();
	bool correctOrder = true;
	for(int i=0;i<a.size();i++)
	{
		if(it->first != expected[i] || it->second != expected[i]+1)
			correctOrder = false;
		++it;
	}
	assertPrint("std::map ordering 4/4:", correctOrder);
	// CHECK-NOT: std::map ordering 4/4: 0
}

void testMultiMap()
{
	std::multimap<int, int> a;
	assertPrint("std::multimap 1/4:", a.size());
	// CHECK: std::multimap 1/4: 0
	a.insert(std::make_pair(1,2));
	assertPrint("std::multimap 2/4:", a.size()); 
	// CHECK: std::multimap 2/4: 1
	a.insert(std::make_pair(10,11));
	a.insert(std::make_pair(99,100));
	a.insert(std::make_pair(100,101));
	a.insert(std::make_pair(100,101));
	a.insert(std::make_pair(-2, -1));
	assertPrint("std::multimap 3/4:", a.size());
	// CHECK: std::multimap 3/4: 6
	int expected[] = {-2, 1, 10, 99, 100, 100};
	std::multimap<int, int>::iterator it = a.begin();
	bool correctOrder = true;
	for(int i=0;i<a.size();i++)
	{
		if(it->first != expected[i] || it->second != expected[i]+1)
			correctOrder = false;
		++it;
	}
	assertPrint("std::multimap ordering 4/4:", correctOrder);
	//CHECK-NOT: std::multimap ordering 4/4: 0
}

void testUnorderedMap()
{
	std::unordered_map<int, int> a;
	assertPrint("std::unordered_map 1/4:", a.size());
	// CHECK: std::unordered_map 1/4: 0
	a.insert(std::make_pair(1,2));
	assertPrint("std::unordered_map 2/4:", a.size());
	// CHECK: std::unordered_map 2/4: 1
	a.insert(std::make_pair(10,11));
	a.insert(std::make_pair(99,100));
	a.insert(std::make_pair(100,101));
	a.insert(std::make_pair(100,101));
	a.insert(std::make_pair(-2, -1));
	assertPrint("std::unordered_map 3/4:", a.size());
	// CHECK: std::unordered_map 3/4: 5
	int expected[] = {-2, 99, 100, 10, 1};
	std::unordered_map<int, int>::iterator it = a.begin();
	bool correctOrder = true;
	for(int i=0;i<a.size();i++)
	{
		if(it->first != expected[i] || it->second != expected[i]+1)
			correctOrder = false;
		++it;
	}
	assertPrint("std::unordered_map ordering 4/4:", correctOrder);
	// CHECK-NOT: std::unordered_map ordering 4/4: 0
}

void testUnorderedMapOrPointers()
{
	std::unordered_map<void*, int> a;
	assertPrint("std::unordered_map of pointers 1/3:", a.size());
	// CHECK: std::unordered_map of pointers 1/3: 0
	a.insert(std::make_pair((void*)NULL,2));
	assertPrint("std::unordered_map of pointers 2/3:", a.size());
	// CHECK: std::unordered_map of pointers 2/3: 1
	int i1,i2,i3,i4;
	a.insert(std::make_pair(&i1,11));
	a.insert(std::make_pair(&i2,100));
	a.insert(std::make_pair(&i3,101));
	a.insert(std::make_pair(&i3,101));
	a.insert(std::make_pair(&i4, -1));
	assertPrint("std::unordered_map of pointers 3/3:", a.size());
	// CHECK: std::unordered_map of pointers 3/3: 5
}

void testPointerMap()
{
	//These report an error as maps of pointers are not supported
	//std::map<void*,int> a;
	//std::multimap<void*,int> m;
}

void webMain()
{
	testMap();
	testMultiMap();
	testUnorderedMap();
	testPointerMap();
	testUnorderedMapOrPointers();
}

