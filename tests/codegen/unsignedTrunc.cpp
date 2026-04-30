//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: regular

// RUN: %compile -cheerp-make-module=es6 -o %t-es6.mjs %s 2>&1
// RUN: python3 %helpers/create_driver.py %t-es6.mjs %S/unsignedTrunc.testing.js %t-es6_driver.mjs --module=es6
// RUN: %node %t-es6_driver.mjs 2>&1 | %FileCheck %s

//CHECK: Module export SUCCESS
#include <tests.h>

namespace [[cheerp::genericjs]] client
{
	bool someFunc();
}

[[cheerp::jsexport]]
int someValue(unsigned char a)
{
	unsigned char X(a);
	X+=23;

	unsigned char d;

	if (client::someFunc())
		d = (unsigned char) (X & (unsigned char)12);
	else
		d = (unsigned char)X;

	return d;
}
