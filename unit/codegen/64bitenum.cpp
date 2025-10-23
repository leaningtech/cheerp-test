// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: 64-bit wide enum 1/3 : SUCCESS
// CHECK: 64-bit wide enum 2/3 : SUCCESS
// CHECK: 64-bit wide enum 3/3 : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2016 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>

enum class BIG_ENUM : uint64_t
{
	SMALL_VALUE = 1,
	BIG_VALUE = 0xffff00000000
};

void webMain()
{
	BIG_ENUM a = BIG_ENUM::SMALL_VALUE;
	assertEqual(1, (int)a, "64-bit wide enum 1/3");
	assertEqual(BIG_ENUM(1), BIG_ENUM::SMALL_VALUE, "64-bit wide enum 2/3");
	volatile BIG_ENUM b = BIG_ENUM::BIG_VALUE;
	assertEqual(a < b, true, "64-bit wide enum 3/3");
}
