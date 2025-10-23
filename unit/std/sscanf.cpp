// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: sscanf 1/4 : SUCCESS
// CHECK: sscanf 2/4 : SUCCESS
// CHECK: sscanf 3/4 : SUCCESS
// CHECK: sscanf 4/4 : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2016 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>
#include <stdio.h>

void webMain()
{
	std::string test = "padding=1,2,3,4";
	int a,b,c,d;
	sscanf(test.c_str(), "padding=%d,%d,%d,%d", &a, &b, &c, &d);
	assertEqual(a, 1, "sscanf 1/4");
	assertEqual(b, 2, "sscanf 2/4");
	assertEqual(c, 3, "sscanf 3/4");
	assertEqual(d, 4, "sscanf 4/4");
}
