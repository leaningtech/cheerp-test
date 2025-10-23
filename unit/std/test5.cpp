// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: Fake mutex support : SUCCESS
// CHECK: Fake atomic support : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2013 Leaning Technlogies
//===----------------------------------------------------------------------===//

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
	assertEqual(true, true, "Fake mutex support");
	//Test atomic
	std::atomic<int> a(1);
	a++;
	a--;
	++a;
	assertEqual(a.load(), 2, "Fake atomic support");
}
