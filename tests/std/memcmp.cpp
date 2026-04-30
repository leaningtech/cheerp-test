//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//


// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck --check-prefix=CHECK_%target %s

#include <tests.h>
#include <string.h>
#include <cheerp/memory.h>

void webMain()
{
	int a = 42;
	int b = 41;
#ifdef __ASMJS__
	assertPrint("memcmp (asmjs/wasm) 1/2:", memcmp(&a, &b, sizeof(int)));
	// CHECK_wasm:  memcmp (asmjs/wasm) 1/2: 1
	// CHECK_asmjs: memcmp (asmjs/wasm) 1/2: 1
#endif
	assertPrint("memcmp (safe) 1/2:", cheerp::memcmp(&a, &b, sizeof(int)));
	// CHECK_js: memcmp (safe) 1/2: 1
	b++;
#ifdef __ASMJS__
	assertPrint("memcmp (asmjs/wasm) 2/2:", memcmp(&a, &b, sizeof(int)));
	// CHECK_wasm:  memcmp (asmjs/wasm) 2/2: 0
	// CHECK_asmjs: memcmp (asmjs/wasm) 2/2: 0
#endif
	assertPrint("memcmp (safe) 2/2:", cheerp::memcmp(&a, &b, sizeof(int)));
	// CHECK_js: memcmp (safe) 2/2: 0
}
