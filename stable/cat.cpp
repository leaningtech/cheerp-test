//===---------------------------------------------------------------------===//
//	Copyright 2022 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t
//
// RUN: regular_only run_if_js %S/test_all_modules.sh %s %S/cat.testing.js %t compile_mode_js 2>&1 | %FileCheck %s 
//
// Also test wasm and asmjs with vanilla driver:
// RUN: regular_only run_if_wasm %valgrind compile_mode_wasm -o %t/w %s 2>&1
// RUN: regular_only run_if_wasm python3 %S/create_driver.py %t/w %S/cat.testing.js %t/w_driver.js --module=vanilla
// RUN: regular_only run_if_wasm %node %t/w_driver.js 2>&1 | %FileCheck %s
//
// RUN: regular_only run_if_asmjs %valgrind compile_mode_asmjs -o %t/a %s 2>&1
// RUN: regular_only run_if_asmjs python3 %S/create_driver.py %t/a %S/cat.testing.js %t/a_driver.js --module=vanilla
// RUN: regular_only run_if_asmjs %node %t/a_driver.js 2>&1 | %FileCheck %s
//
// CHECK: Print something to console
// CHECK-NOT: unexpected exit : FAILURE

#include <tests.h>
#include <cheerp/client.h> 

class [[cheerp::jsexport]] Cat {
public:
	[[cheerp::genericjs]]
	Cat(client::String* name) : age(name->operator int()) {}
	int age;

	[[cheerp::genericjs]]
	~Cat() {
		client::console.log("Print something to console");
	}
	int i0() {
		return age * age;
	}
};
