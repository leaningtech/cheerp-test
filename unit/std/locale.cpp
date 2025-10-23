// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: setlocale 1/4 : SUCCESS
// CHECK: setlocale 2/4 : SUCCESS
// CHECK: setlocale 3/4 : SUCCESS
// CHECK: setlocale 4/4 : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2017 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>

#include <clocale>

void webMain()
{
	const char* loc;
	loc = setlocale(LC_ALL,"C");
	assertEqual(loc, "C", "setlocale 1/4");

	loc = setlocale(LC_ALL,"C.UTF-8");
	assertEqual(loc, "C.UTF-8;C;C;C;C;C", "setlocale 2/4");

	// Go back to default ("C.UTF-8")
	loc = setlocale(LC_ALL,"");
	assertEqual(loc, "C.UTF-8;C;C;C;C;C", "setlocale 3/4");

	// Invalid locale
	loc = setlocale(LC_ALL,"GARBAGE");
	assertEqual(loc, "GARBAGE", "setlocale 4/4");
}
