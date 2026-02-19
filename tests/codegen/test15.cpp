//===---------------------------------------------------------------------===//
//      Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_js compile_mode_js -o %t/j %s 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 | %FileCheck %s

#include <tests.h>

struct A
{
	int a;
};

struct B
{
	A* a;
};

void __attribute__((noinline)) func(A** a)
{
	*a = NULL;
}

void webMain()
{
	B b { new A() };
	func(&b.a);
	A* volatile a2 = b.a;
	assertEqual<A *>((A *)a2, (A *)nullptr, "Pointers to structs in structs: SUCCESS");
	//CHECK: Pointers to structs in structs: SUCCESS
}