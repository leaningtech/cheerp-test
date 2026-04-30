//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s

// CHECK: Rewriting i64 in types and expressions 1/2: 0xf00000000000001
// CHECK: Rewriting i64 in types and expressions 2/2: 0xf00000000000001

#include <tests.h>

struct Foo {
	int i;
	long long j;
};

int i = 1;
Foo fg{1,(long long)&i+0x0f00000000000001-(long long)&i};

void webMain()
{
	assertPrint("Rewriting i64 in types and expressions 1/2:", Hex((unsigned long long)fg.j));
	assertPrint("Rewriting i64 in types and expressions 2/2:", Hex((unsigned long long)(long long)&fg + 0x0f00000000000001 - (long long)&fg));
}