//===---------------------------------------------------------------------===//
//      Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_js compile_mode_js -o %t/j %s 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 | %FileCheck %s

//CHECK: Multibyte string support: 5

#include <tests.h>
#include <wchar.h>
#include <locale.h>

void webMain()
{
    setlocale(LC_ALL, "C.UTF-8");
	const char * h = "hello";
	mbstate_t s{};
	size_t ret=mbsrtowcs(0, &h, 0, &s);

	assertPrint<size_t>("Multibyte string support:", ret);
}
