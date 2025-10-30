//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_js compile_mode_js -o %t/j %s 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 | %FileCheck %s

// CHECK: 64-bit wide enum 1/3: 1
// CHECK: 64-bit wide enum 2/3: SUCCESS
// CHECK: 64-bit wide enum 3/3: SUCCESS

#include <tests.h>

enum class BIG_ENUM : uint64_t
{
	SMALL_VALUE = 1,
	BIG_VALUE = 0xffff00000000
};

void webMain()
{
	BIG_ENUM a = BIG_ENUM::SMALL_VALUE;
	assertPrint("64-bit wide enum 1/3:", (int)a);
	if (BIG_ENUM(1) == BIG_ENUM::SMALL_VALUE)
		assertPrint("64-bit wide enum 2/3: SUCCESS");
	volatile BIG_ENUM b = BIG_ENUM::BIG_VALUE;
	if (a < b)
		assertPrint("64-bit wide enum 3/3: SUCCESS");
}
