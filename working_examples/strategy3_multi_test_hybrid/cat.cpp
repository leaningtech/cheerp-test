//===---------------------------------------------------------------------===//
//	Copyright 2022 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t

// Preexecution mode (PREEXECUTE_MODE=true):
// RUN: preexec_only run_if_js %valgrind compile_mode_js -o %t/j_pre %s 2>&1 | %FileCheck %s --check-prefix=PREEXEC_OK
// RUN: preexec_only run_if_asmjs %valgrind compile_mode_asmjs -o %t/a_pre %s 2>&1 | %FileCheck %s --check-prefix=PREEXEC_OK
//
// Regular mode (PREEXECUTE_MODE=false):
// RUN: regular_only run_if_wasm %valgrind compile_mode_wasm -o %t/w %s 2>&1 && %node %t/w 2>&1 | %FileCheck %s --check-prefix=REG_OK
// RUN: regular_only run_if_js %valgrind compile_mode_js -o %t/j %s 2>&1 && %node %t/j 2>&1 | %FileCheck %s --check-prefix=REG_OK
// RUN: regular_only run_if_asmjs %valgrind compile_mode_asmjs -o %t/a %s 2>&1 && %node %t/a 2>&1 | %FileCheck %s --check-prefix=REG_OK
//
// PREEXEC_OK-NOT: error
// REG_OK-NOT: unexpected exit : FAILURE

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
