//===---------------------------------------------------------------------===//
//	Copyright 2026 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>

unsigned arr[] = {5, 5, 9, 5, 8, 5, 50};
unsigned matrix[3][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};
struct holder
{
	unsigned before;
	unsigned values[5];
} h = {99, {1, 2, 3, 4, 70}};

void webMain()
{
	// Test loops walking a pointer that starts past the first element of an array
	unsigned last = 0;
	unsigned* p = arr + 1;
	for (int i = 0; i < 6; i++)
		last = *p++;
	assertPrint("Array offset loop 1/3:", last);
	// CHECK: Array offset loop 1/3: 50

	last = 0;
	p = &matrix[1][1];
	for (int i = 0; i < 3; i++)
		last = *p++;
	assertPrint("Array offset loop 2/3:", last);
	// CHECK: Array offset loop 2/3: 8

	last = 0;
	p = &h.values[2];
	for (int i = 0; i < 3; i++)
		last = *p++;
	assertPrint("Array offset loop 3/3:", last);
	// CHECK: Array offset loop 3/3: 70
}
