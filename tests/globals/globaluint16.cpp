//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>
#include <stdint.h>
#include <stdio.h>

uint16_t global = 123;

void doSomething()
{
	char buffer[100];
	sprintf(buffer, "%d\n", global);

	int len = strlen(buffer);
	assertPrint("Global uint16_t length check:", (len < 7) ? 1 : 0);
}

int main()
{
	for (int i=0; i<53; i++)
	{
		global *= global;
		doSomething();
	}
	return global;
}

// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
