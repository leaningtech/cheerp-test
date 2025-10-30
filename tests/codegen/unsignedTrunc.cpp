//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

//REQUIRES: regular

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -cheerp-make-module=es6 -o %t/j_es6.mjs %s 2>&1 
// RUN: regular_only run_if_js python3 %S/../helpers/create_driver.py %t/j_es6.mjs %S/unsignedTrunc.testing.js %t/j_es6_driver.mjs --module=es6
// RUN: regular_only run_if_js %node %t/j_es6_driver.mjs %s 2>&1 | %FileCheck %s

// RUN: regular_only run_if_wasm compile_mode_wasm -cheerp-make-module=es6 -o %t/w_es6.mjs %s 2>&1
// RUN: regular_only run_if_wasm python3 %S/../helpers/create_driver.py %t/w_es6.mjs %S/unsignedTrunc.testing.js %t/w_driver.mjs --module=es6
// RUN: regular_only run_if_wasm %node %t/w_driver.mjs 2>&1 | %FileCheck %s

// RUN: regular_only run_if_asmjs compile_mode_asmjs -cheerp-make-module=es6 -o %t/a_es6.mjs %s 2>&1
// RUN: regular_only run_if_asmjs python3 %S/../helpers/create_driver.py %t/a_es6.mjs %S/unsignedTrunc.testing.js %t/a_driver.mjs --module=es6
// RUN: regular_only run_if_asmjs %node %t/a_driver.mjs 2>&1 | %FileCheck %s

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
