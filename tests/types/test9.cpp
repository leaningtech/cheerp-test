//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: mkdir -p %t

// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1| %FileCheck %s
// RUN: preexec_only run_if_js compile_mode_js -o %t/j %s 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 | %FileCheck %s

#include <tests.h>

class A
{
public:
	int a;
};

class B: public A
{
public:
	int b;
};

class C: public B
{
public:
	int c;
};

void webMain()
{
	C c;
	B* b = &c;
	A* a = &c;
	B* b2 = static_cast<B*>(a);
	assertPrint("Pointer equality for classes with downcast array:", (signed int)(b == b2));
	// CHECK: Pointer equality for classes with downcast array: 1
}
