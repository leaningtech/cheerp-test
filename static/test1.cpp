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
	A():a(42)
	{
		assertPrint("Static initialisation of class instances 1/3:", a);
		// assertEqual(true, true, "Static initialiation of class instances  1/3");
	}
};

int f()
{
	static A a;
	a.a++;
	return a.a;
}

void webMain()
{	
	assertPrint("Static initialisation of class instances 2/3:", f());
	// CHECK: Static initialisation of class instances 2/3: 43
	assertPrint("Static initialisation of class instances 3/3:", f());
	// CHECK: Static initialisation of class instances 3/3: 44
}
