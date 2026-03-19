//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: asmjs

// RUN: mkdir -p %t
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1 | %FileCheck %s



#include <tests.h>

struct Foo {
	int i;
	Foo* next;
	Foo(int i, Foo* next): i(i), next(next)
	{}
};

[[cheerp::genericjs]]
int process_foo(const Foo& f)
{
	return f.i+ (f.next == nullptr? 0 : process_foo(*f.next));
}

void webMain()
{
	Foo f(1,new Foo(2, new Foo(3, nullptr)));
	assertPrint("ffi store support for RAW pointers:", process_foo(f));
	// CHECK: ffi store support for RAW pointers: 6
}
