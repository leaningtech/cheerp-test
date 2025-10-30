// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s

//CHECK: Multibyte string support : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2015 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>
#include <wchar.h>
#include <locale.h>

void webMain()
{
    setlocale(LC_ALL, "C.UTF-8");
	const char * h = "hello";
	mbstate_t s{};
	size_t ret=mbsrtowcs(0, &h, 0, &s);

	assertEqual<size_t>(ret, 5, "Multibyte string support");
}
