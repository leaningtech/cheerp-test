//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_js compile_mode_js -o %t/j %s 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 | %FileCheck %s

#include <tests.h>

void webMain()
{
	volatile float f1 = -0x80000000;
	volatile float f2 = 0x7fffff80;
	volatile float f3 = 0;
	volatile float f4 = f1 * 2;
	volatile float f5 = 0x80000000;
#ifndef __WASM__
	assertPrint("Float to int (signed) 1/3: ", Hex(int32_t(f1)));
#endif
	assertPrint("Float to int (signed) 2/3:", Hex(int32_t(f2)));
	assertPrint("Float to int (signed) 3/3:", int32_t(f3));
#ifndef __WASM__
	volatile int32_t fi4 = int32_t(f4); // Check for runtime crash
	volatile int32_t fi5 = int32_t(f5); // Check for runtime crash
#endif
	volatile uint32_t u1 = uint32_t(f1); // Check for runtime crash
	assertPrint<uint32_t>("Float to int (unsigned) 1/3:", Hex(uint32_t(f2)));
	assertPrint<uint32_t>("Float to int (unsigned) 2/3:", uint32_t(f3));
#ifndef __WASM__
	volatile uint32_t fu4 = uint32_t(f4); // Check for runtime crash
#endif
	assertPrint<uint32_t>("Float to int (unsigned) 3/3:", Hex(uint32_t(f5)));

	volatile double d1 = -0x80000000;
	volatile double d2 = 0x7fffffff;
	volatile double d3 = 0;
	volatile double d4 = d1 * 2;
	volatile double d5 = 0x80000000;
#ifndef __WASM__
	assertPrint<int32_t>("Double to int (signed) 1/3:", Hex(int32_t(d1)));
#endif
	assertPrint<int32_t>("Double to int (signed) 2/3:", Hex(int32_t(d2)));
	assertPrint<int32_t>("Double to int (signed) 3/3:", int32_t(d3));
#ifndef __WASM__
	volatile int32_t di4 = int32_t(d4); // Check for runtime crash
	volatile int32_t di5 = int32_t(d5); // Check for runtime crash
#endif
	volatile uint32_t du1 = uint32_t(d1); // Check for runtime crash
	assertPrint<uint32_t>("Double to int (unsigned) 1/3:", Hex(uint32_t(d2)));
	assertPrint<uint32_t>("Double to int (unsigned) 2/3:", uint32_t(d3));
#ifndef __WASM__
	volatile uint32_t du4 = uint32_t(d4); // Check for runtime crash
#endif
	assertPrint<uint32_t>("Double to int (unsigned) 3/3:", Hex(uint32_t(d5)));
}


//CHECK-JS: Float to int (signed) 1/3: -0x80000000
//CHECK-WASM-NOT: Float to int (signed) 1/3: 0x7fffff80

//CHECK: Float to int (signed) 2/3: 0x7fffff80
//CHECK: Float to int (signed) 3/3: 0
//CHECK: Float to int (unsigned) 1/3: 0x7fffff80
//CHECK: Float to int (unsigned) 2/3: 0
//CHECK: Float to int (unsigned) 3/3: 0x80000000

//CHECK-JS: Double to int (signed) 1/3: -0x80000000
//CHECK-WASM-NOT: Double to int (signed) 1/3: 0x7fffff80

//CHECK: Double to int (signed) 2/3: 0x7fffffff
//CHECK: Double to int (signed) 3/3: 0
//CHECK: Double to int (unsigned) 1/3: 0x7fffffff
//CHECK: Double to int (unsigned) 2/3: 0
//CHECK: Double to int (unsigned) 3/3: 0x80000000