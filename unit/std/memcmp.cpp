// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s

//===---------------------------------------------------------------------===//
//	Copyright 2018 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>
#include <string.h>
#include <cheerp/memory.h>

void webMain()
{
	int a = 42;
	int b = 41;
#ifdef __ASMJS__
	assertEqual(memcmp(&a,&b,sizeof(int)), 1, "memcmp (asmjs/wasm) 1/2");
#endif
	assertEqual(cheerp::memcmp(&a,&b,sizeof(int)), 1, "memcmp (safe) 1/2");
	b++;
#ifdef __ASMJS__
	assertEqual(memcmp(&a,&b,sizeof(int)), 0, "memcmp (asmjs/wasm) 2/2");
#endif
	assertEqual(cheerp::memcmp(&a,&b,sizeof(int)), 0, "memcmp (safe) 2/2");
}
