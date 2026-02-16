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

const int N = 2301;

float a[N];

int main()
{
	for (int i=0; i<N; i++)
		a[i] = i*i/1.23;
	for (int i=N-2; i>=0; i--)
		a[i] += a[i+1] * (i%2 ? 1.0 : -1.0);

	float acc = 123;
  int numSamples = (int)(N*0.91);
  for(int i = 0; i < numSamples; ++i)
    acc += a[N-1-i];

	assertPrint("reverse iteration of static array 1/2:", (int)a[0]);
	// CHECK: reverse iteration of static array 1/2: 4302680
	assertPrint("reverse iteration of static array 2/2:", (int)acc);
	// CHECK: reverse iteration of static array 2/2: 1649593728
	return 0;
}
