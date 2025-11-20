// Test Cat class with jsexport across all targets
// RUN: mkdir -p %t
// RUN: %compile_for{wasm} -O1 -o %t/w %s
// RUN: %compile_for{js} -O1 -o %t/j %s
// RUN: %compile_for{asmjs} -O1 -o %t/a %s
//
// RUN: %run_if{wasm} test -f %t/w && test -f %t/w.wasm
// RUN: %run_if{js} test -f %t/j
// RUN: %run_if{asmjs} test -f %t/a
//
// RUN: %run_if{wasm} %node %t/w 2>&1
// RUN: %run_if{js} %node %t/j 2>&1
// RUN: %run_if{asmjs} %node %t/a 2>&1

//===---------------------------------------------------------------------===//
//	Copyright 2022 Leaning Technologies
//===----------------------------------------------------------------------===//

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
