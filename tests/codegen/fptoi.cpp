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
#include <limits>

template<typename F, typename I>
constexpr I roundTrip(I i)
{
	F f = static_cast<F>(i);
	return static_cast<I>(f);
}
template<typename F, typename I, I max>
void testFPToI(int testNo)
{
	constexpr I maxExpected = roundTrip<F>(max);
	I maxi = roundTrip<F>(unitBlackBox(max));
	assertPrint("fptoi test number:", testNo);
	assertPrint("fptoi support maxi:", Hex(maxi));
	assertPrint("fptoi support maxExpected:", Hex(maxExpected));
}
void webMain()
{
	testFPToI<float, int, 0x7fffff80>(1);
	// CHECK: fptoi test number: 1
	// CHECK: fptoi support maxi: 0x7fffff80
	// CHECK: fptoi support maxExpected: 0x7fffff80
	testFPToI<float, unsigned int, 0xffffff00>(2);
	// CHECK: fptoi test number: 2
	// CHECK: fptoi support maxi: 0xffffff00
	// CHECK: fptoi support maxExpected: 0xffffff00
	testFPToI<float, long long, 0x7fffff8000000000>(3);
	// CHECK: fptoi test number: 3
	// CHECK: fptoi support maxi: 0x7fffff8000000000
	// CHECK: fptoi support maxExpected: 0x7fffff8000000000
	testFPToI<float, unsigned long long, 0xffffff0000000000>(4);
	// CHECK: fptoi test number: 4
	// CHECK: fptoi support maxi: 0xffffff0000000000
	// CHECK: fptoi support maxExpected: 0xffffff0000000000	
	testFPToI<double, int, 0x7fffffff>(5);
	// CHECK: fptoi test number: 5
	// CHECK: fptoi support maxi: 0x7fffffff
	// CHECK: fptoi support maxExpected: 0x7fffffff
	testFPToI<double, unsigned int, 0xffffffff>(6);
	// CHECK: fptoi test number: 6
	// CHECK: fptoi support maxi: 0xffffffff
	// CHECK: fptoi support maxExpected: 0xffffffff
	testFPToI<double, long long, 0x7ffffffffffffc00>(7);
	// CHECK: fptoi test number: 7
	// CHECK: fptoi support maxi: 0x7ffffffffffffc00
	// CHECK: fptoi support maxExpected: 0x7ffffffffffffc00
	testFPToI<double, unsigned long long, 0xfffffffffffff800>(8);
	// CHECK: fptoi test number: 8
	// CHECK: fptoi support maxi: 0xfffffffffffff800
	// CHECK: fptoi support maxExpected: 0xfffffffffffff800
}
