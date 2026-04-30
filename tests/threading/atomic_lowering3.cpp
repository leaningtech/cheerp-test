//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// REQUIRES: regular
// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s



#include <tests.h>
#include <atomic>

std::atomic<int> value(0);

void addToNumber()
{
	int num = value.load();
	int newNum = num + 1;
	while (!value.compare_exchange_weak(num, newNum))
		newNum = num + 1;
}

int main()
{
	for (int i = 0; i < 100; i++)
		addToNumber();
	assertPrint("Simple compare exchange:", value.load());
	//CHECK: Simple compare exchange: 100
}
