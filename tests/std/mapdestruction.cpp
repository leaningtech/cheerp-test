//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>
#include <map>
#include <unordered_map>

struct A
{
	~A()
	{
		destructionCount++;
	}
	static int destructionCount;
};

int A::destructionCount = 0;

void webMain()
{
	std::map<int, A> m1;
        m1[1];
	m1.clear();
	assertPrint("Destructors in std::map 1/2:", A::destructionCount);
	//CHECK: Destructors in std::map 1/2: 1
	std::unordered_map<int, A> m2;
        m2[1];
	m2.clear();
	assertPrint("Destructors in std::map 2/2:", A::destructionCount);
	//CHECK: Destructors in std::map 2/2: 2
}

