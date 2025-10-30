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
#include <map>
#include <unordered_map>

struct A
{
	~A()
	{
		destructionCount++;
	}
	static int destructionCount;
};

int A::destructionCount = 0;

void webMain()
{
	std::map<int, A> m1;
        m1[1];
	m1.clear();
	assertPrint("Destructors in std::map 1/2:", A::destructionCount);
	//CHECK: Destructors in std::map 1/2: 1
	std::unordered_map<int, A> m2;
        m2[1];
	m2.clear();
	assertPrint("Destructors in std::map 2/2:", A::destructionCount);
	//CHECK: Destructors in std::map 2/2: 2
}

