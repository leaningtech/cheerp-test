//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: regular

// RUN: %compile -cheerp-make-module=es6 -fexceptions -o %t-es6.mjs %s 2>&1
// RUN: python3 %helpers/create_driver.py %t-es6.mjs %S/exitexception1.testing.js %t-es6_driver.mjs --module=es6
// RUN: %node %t-es6_driver.mjs 2>&1 | %FileCheck %s

// RUN: %compile -cheerp-make-module=commonjs -fexceptions -o %t-commonjs.js %s 2>&1
// RUN: python3 %helpers/create_driver.py %t-commonjs.js %S/exitexception1.testing.js %t-commonjs_driver.js --module=commonjs
// RUN: %node %t-commonjs_driver.js 2>&1 | %FileCheck %s

//CHECK: Start execution : SUCCESS
//CHECK-NOT: FAILURE

#include <tests.h>
#include <cheerp/client.h>

int main()
{
	cheerp::console_log("Start execution : SUCCESS");
	int i = unitBlackBox(1);
	try
	{
		if (i == 1)
			exit(0);
	} catch(cheerp::JSException& e) {
		cheerp::console_log("JSException clause ignores exit-exception : FAILURE");
	} catch(...) {
		cheerp::console_log("Catch-all clause ignores exit-exception : FAILURE");
	}
	cheerp::console_log("Stop execution on exit : FAILURE");
}
