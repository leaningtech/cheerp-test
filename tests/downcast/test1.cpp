//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t

// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1| %FileCheck %s
// RUN: preexec_only run_if_js compile_mode_js -o %t/j %s 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 | %FileCheck %s

// CHECK: Downcast 1/2: 2
// CHECK: Downcast 2/2: 2

#include "../tests.h"

struct C
{
	float c;
	C():c(3){}
};

struct B
{
	int b;
	B():b(1){}
};

struct A: public B, public C
{
	int a;
	A():a(2){}
};

void webMain()
{
	A a;
	B* volatile b=&a;
	C* volatile c=&a;
	A* volatile a2=static_cast<A*>(b);
	assertPrint("Downcast 1/2:", a2->a);
	A* volatile a3=static_cast<A*>(c);
	assertPrint("Downcast 2/2:", a3->a);
}
