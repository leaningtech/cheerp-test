//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// (es6 driver tests)
// REQUIRES: regular
// RUN: %run_if_js %compile -cheerp-make-module=es6 -fexceptions -o %t-j_es6.mjs %s 2>&1 
// RUN: %run_if_js python3 %helpers/create_driver.py %t-j_es6.mjs %S/exitexception2.testing.js %t-j_es6_driver.mjs --module=es6
// RUN: %run_if_js %node %t-j_es6_driver.mjs %s 2>&1 | %FileCheck %s

// RUN: %run_if_wasm %compile -cheerp-make-module=es6 -fexceptions -o %t-w_es6.mjs %s 2>&1
// RUN: %run_if_wasm python3 %helpers/create_driver.py %t-w_es6.mjs %S/exitexception2.testing.js %t-w_es6_driver.mjs --module=es6
// RUN: %run_if_wasm %node %t-w_es6_driver.mjs 2>&1 | %FileCheck %s

// RUN: %run_if_asmjs %compile -cheerp-make-module=es6 -fexceptions -o %t-a_es6.mjs %s 2>&1
// RUN: %run_if_asmjs python3 %helpers/create_driver.py %t-a_es6.mjs %S/exitexception2.testing.js %t-a_es6_driver.mjs --module=es6
// RUN: %run_if_asmjs %node %t-a_es6_driver.mjs 2>&1 | %FileCheck %s

// (commonjs driver tests)
// RUN: %run_if_js %compile -cheerp-make-module=commonjs -fexceptions -o %t-j_commonjs.js %s 2>&1 
// RUN: %run_if_js python3 %helpers/create_driver.py %t-j_commonjs.js %S/exitexception2.testing.js %t-j_commonjs_drives.js --module=commonjs
// RUN: %run_if_js %node %t-j_commonjs_drives.js %s 2>&1 | %FileCheck %s

// RUN: %run_if_wasm %compile -cheerp-make-module=commonjs -fexceptions -o %t-w_commonjs.js %s 2>&1
// RUN: %run_if_wasm python3 %helpers/create_driver.py %t-w_commonjs.js %S/exitexception2.testing.js %t-w_commonjs_drives.js --module=commonjs
// RUN: %run_if_wasm %node %t-w_commonjs_drives.js 2>&1 | %FileCheck %s

// RUN: %run_if_asmjs %compile -cheerp-make-module=commonjs -fexceptions -o %t-a_commonjs.js %s 2>&1
// RUN: %run_if_asmjs python3 %helpers/create_driver.py %t-a_commonjs.js %S/exitexception2.testing.js %t-a_commonjs_driver.js --module=commonjs
// RUN: %run_if_asmjs %node %t-a_commonjs_driver.js 2>&1 | %FileCheck %s

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
			exit(1);
	} catch(cheerp::JSException& e) {
		cheerp::console_log("JSException clause ignores exit-exception : FAILURE");
	} catch(...) {
		cheerp::console_log("Catch-all clause ignores exit-exception : FAILURE");
	}
	cheerp::console_log("Stop execution on exit : FAILURE");
}
