// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: Range for on std::array 1/3 : SUCCESS
// CHECK: Range for on std::array 2/3 : SUCCESS
// CHECK: Range for on std::array 3/3 : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2014 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>
#include <array>
#include <cstdio>

std::array<int,2> arr({{1, 2}});

void webMain()
{
	int index = 0;
	for (const auto & elem: arr)
	{
		if(index==0)
			assertEqual(elem, 1, "Range for on std::array 1/3");
		else if(index==1)
			assertEqual(elem, 2, "Range for on std::array 2/3");
		index++;
	}
	assertEqual(index, 2, "Range for on std::array 3/3");
}
