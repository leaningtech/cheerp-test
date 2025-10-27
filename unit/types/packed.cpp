//===---------------------------------------------------------------------===//
//	Copyright 2021 Leaning Technlogies
//===----------------------------------------------------------------------===//

//REQUIRES: wasm

// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: Sizeof support : SUCCESS

#include <tests.h>

struct __attribute__((packed)) MyStruct{
    char c;
    int a;
};

int main()
{
	assertEqual(sizeof(MyStruct), 5u, "Sizeof support");
	return 0;
}

