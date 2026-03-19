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
#include <mutex>
#include <atomic>
//Including this should cause an error
//#include <thread>

void webMain()
{
	//Test mutex
	std::mutex m;
	m.lock();
	m.unlock();
	assertPrint("Fake mutex support:", 1);
	// CHECK: Fake mutex support: 1 
	//Test atomic
	std::atomic<int> a(1);
	a++;
	a--;
	++a;
	assertPrint("Fake atomic support:", a.load());
	// CHECK: Fake atomic support: 2
}
