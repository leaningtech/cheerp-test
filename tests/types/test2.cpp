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

struct S
{
	int i;
};

struct T
{
	float f;
};

void webMain()
{
	//Test casting to void*
	void* volatile opaque1 = (void*)0;
	float a = 0.2f;

	opaque1 = &a;

	float* pa = (float*)opaque1;
	// assertEqual(*pa, 0.2f, "Float, cast to void* and back");
	assertPrint("Float, cast to void* and back:", *pa);
	// CHECK: Float, cast to void* and back: 0.20000000

	S s;
	s.i = 42;
	opaque1 = &s;

	S* ps = (S*)opaque1;
	// assertEqual(ps->i, 42, "Structure, cast to void* and back 1/2");
	assertPrint("Structure, cast to void* and back 1/2:", ps->i);
	// CHECK: Structure, cast to void* and back 1/2: 42

	T t;
	t.f = 42;
	opaque1 = &t;

	T* pt = (T*)opaque1;
	assertEqual(pt->f, 42.f, "Structure, cast to void* and back 2/2");
	assertPrint("Structure, cast to void* and back 2/2:", pt->f);
	// CHECK: Structure, cast to void* and back 2/2: 42.000000000
}
