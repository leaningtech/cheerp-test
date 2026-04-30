//===---------------------------------------------------------------------===//
//      Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s

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
