//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>
#include <list>

using namespace std;

void webMain()
{
	struct A
	{
		list<int> v;
		int a,b;
	} obj;

	list<A> l;

	l.push_back(move(obj));
	assertPrint("List push_back 1/2:", l.size());
	// CHECK: List push_back 1/2: 1
	l.push_back(obj);
	assertPrint("List push_back 2/2:", l.size());
	// CHECK: List push_back 2/2: 2
}
