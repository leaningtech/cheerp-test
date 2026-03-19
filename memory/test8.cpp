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
	A(int _a):a(_a)
	{
	}
};

void webMain()
{
	//Test new with [[cheerp::nonit]]
	//Note that A has no default constructor
	A* a=new A[2] [[cheerp::noinit]];
	a[1].a = 42;
	assertPrint("New array with [[cheerp::noinit]]:", a[1].a);
	// CHECK: New array with {{\[\[}}cheerp::noinit{{\]\]}}: 42
	A(*a2)[3]=new A[2][3] [[cheerp::noinit]];
	a2[1][1].a = 43;
	assertPrint("New multi array with [[cheerp::noinit]]:", a2[1][1].a);
	// CHECK: New multi array with {{\[\[}}cheerp::noinit{{\]\]}}: 43
	A* a3=new A [[cheerp::noinit]];
	a3->a = 44;
	assertPrint("New with [[cheerp::noinit]]:", a3->a);
	// CHECK: New with {{\[\[}}cheerp::noinit{{\]\]}}: 44
}
