//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 %then_run_js | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_js compile_mode_js -o %t/j %s 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 | %FileCheck %s


#include <tests.h>

uint16_t a = 0x11AA;

void webMain()
{
	//Test that bswaps are properly handled
	volatile uint16_t b = __builtin_bswap16(a);

        uint16_t b2 = b;

	assertPrint("bswap support 1/2:", Hex(a));
	// CHECK: bswap support 1/2: 0x11aa
	assertPrint("bswap support 2/2:", Hex(b2));
	// CHECK: bswap support 2/2: 0xaa11
}
