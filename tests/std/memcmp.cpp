//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//


// RUN: %run_if_js %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s
// RUN: %run_if_wasm %compile -o %t.js %s 2>%t.log && %run | %FileCheck --check-prefix=CHECK_WASM %s
// RUN: %run_if_asmjs %compile -o %t.js %s 2>%t.log && %run | %FileCheck --check-prefix=CHECK_WASM %s

#include <tests.h>
#include <string.h>
#include <cheerp/memory.h>

void webMain()
{
	int a = 42;
	int b = 41;
#ifdef __ASMJS__
	assertPrint("memcmp (asmjs/wasm) 1/2:", memcmp(&a, &b, sizeof(int)));
	//CHECK_WASM: memcmp (asmjs/wasm) 1/2: 1
#endif
	assertPrint("memcmp (safe) 1/2:", cheerp::memcmp(&a, &b, sizeof(int)));
	// CHECK: memcmp (safe) 1/2: 1
	b++;
#ifdef __ASMJS__
	assertPrint("memcmp (asmjs/wasm) 2/2:", memcmp(&a, &b, sizeof(int)));
	// CHECK_WASM: memcmp (asmjs/wasm) 2/2: 0
#endif
	assertPrint("memcmp (safe) 2/2:", cheerp::memcmp(&a, &b, sizeof(int)));
	// CHECK: memcmp (safe) 2/2: 0
}
