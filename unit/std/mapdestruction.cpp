// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: Destructors in std::map 1/2 : SUCCESS
// CHECK: Destructors in std::map 2/2 : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2017 Leaning Technlogies
//===----------------------------------------------------------------------===//

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
	assertEqual(A::destructionCount, 1, "Destructors in std::map 1/2");
	std::unordered_map<int, A> m2;
        m2[1];
	m2.clear();
	assertEqual(A::destructionCount, 2, "Destructors in std::map 2/2");
}
