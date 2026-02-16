//===----------------------------------------------------------------------===//
//      Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: js, regular

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s

#include <tests.h>

// Test merging of arrays of the same type inside structs

struct A
{
	char* p1[4];
	char c1[4];
	char* p2[3];
	char c2[3];
};

A globalA = { { NULL, NULL, NULL, NULL }, { 0, 1, 2, 3} };

void webMain()
{
	A a;
	for(int i=0;i<4;i++)
	{
		a.c1[i] = i;
		a.p1[i] = &a.c1[i];
	}
	for(int i=0;i<3;i++)
	{
		a.c2[i] = i+10;
		a.p2[i] = &a.c2[i];
	}
	// The next lines can succeed only if p2 is merged in p1
	char** tmp1=a.p1;
	if (tmp1[4] == &a.c2[0])
		assertPrint("Merging arrays in structs 1/4:", "SUCCESS"); // CHECK: Merging arrays in structs 1/4: SUCCESS
	else
		__preexecute_print_case("FAILURE");
	if (*tmp1[4] == a.c2[0])
		assertPrint("Merging arrays in structs 2/4:", "SUCCESS"); // CHECK: Merging arrays in structs 2/4: SUCCESS
	else
		__preexecute_print_case("FAILURE");

	// The next line can succeed only if c2 is merged in c1
	char* tmp2=a.c1;
	if (tmp2[4] == a.c2[0])
		assertPrint("Merging arrays in structs 3/4:", "SUCCESS"); // CHECK: Merging arrays in structs 3/4: SUCCESS
	else
		__preexecute_print_case("FAILURE");
	if (tmp2[4] == (char)10)
		assertPrint("Merging arrays in structs 4/4:", "SUCCESS"); // CHECK: Merging arrays in structs 4/4: SUCCESS
	else
		__preexecute_print_case("FAILURE");
}
