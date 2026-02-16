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
#include <list>

using namespace std;

void webMain()
{
	struct A
	{
		list<int> v;
		int a,b;
	} obj;

	list<A> l;

	l.push_back(move(obj));
	assertPrint("List push_back 1/2:", l.size());
	// CHECK: List push_back 1/2: 1
	l.push_back(obj);
	assertPrint("List push_back 2/2:", l.size());
	// CHECK: List push_back 2/2: 2
}
