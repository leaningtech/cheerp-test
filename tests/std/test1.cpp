//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>
#include <list>

namespace {

struct Point
{
	int x;
	int y;
};

};

void webMain()
{
	std::list<Point> l;
	assertPrint("List construction:", l.size());
	// CHECK: List construction: 0
	l.insert(l.begin(),Point{.x=1,.y=2});
	assertPrint("List construction:", l.size());
	// CHECK: List construction: 1
}
