//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// REQUIRES: packed_tests, regular


// RUN: %run_if_js %compile -o %t-j %s 2>&1 && node %t-j 2>&1 | %FileCheck %s
// RUN: %run_if_wasm %compile -o %t-w %s 2>&1 && node %t-w 2>&1 | %FileCheck %s

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

