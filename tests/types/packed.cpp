//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// REQUIRES: packed_tests, regular


// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s

#include <tests.h>

struct __attribute__((packed)) MyStruct{
    char c;
    int a;
};

int main()
{
    assertPrint("Sizeof support:", sizeof(MyStruct));
    // CHECK: Sizeof support: 5
	return 0;
}

