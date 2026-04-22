//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technlogies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 %then_run_js | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_js compile_mode_js -o %t/j %s 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 | %FileCheck %s

#include <tests.h>
#include <cmath>

template <class T>
int computeFunc(int A)
{
	T B = A;
	B *= A;
	B *= A;
	B = std::abs(B);
	return B % T(123123);
}

int main()
{
	int X = 0;
	for (int i=-100; i<100; i++)
		X += computeFunc<long long>(i) + computeFunc<long long>(i << 5) + computeFunc<long long>(i * 1e5 + 12);
	assertPrint("Absolute value summation 1/3:", X);
	// CHECK: Absolute value summation 1/3: 32038773

	int Y = 0;
	for (int i=-100; i<100; i++)
		Y += computeFunc<int>(i) + computeFunc<int>(i << 5) + computeFunc<int>(i * 1e5 + 12);
	assertPrint("Absolute value summation 2/3:", Y);
	// CHECK: Absolute value summation 2/3: 33131675

	int Z = 0;
	for (int i=-100; i<100; i++)
		Z += computeFunc<char>(i) + computeFunc<char>(i << 5) + computeFunc<char>(i * 1e5 + 12);
	assertPrint("Absolute value summation 3/3:", Z);
	// CHECK: Absolute value summation 3/3: 3542
}
