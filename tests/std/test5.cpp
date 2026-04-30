//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>
#include <mutex>
#include <atomic>
//Including this should cause an error
//#include <thread>

void webMain()
{
	//Test mutex
	std::mutex m;
	m.lock();
	m.unlock();
	assertPrint("Fake mutex support:", 1);
	// CHECK: Fake mutex support: 1 
	//Test atomic
	std::atomic<int> a(1);
	a++;
	a--;
	++a;
	assertPrint("Fake atomic support:", a.load());
	// CHECK: Fake atomic support: 2
}
