//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>

static char *var1[] = { "\0" };

char **var2 = &var1[0];

void webMain()
{
	char* a=*var2;
	assertPrint("Access to pointers of global arrays:", (*a == '\0'));
	// CHECK: Access to pointers of global arrays: 1
}
