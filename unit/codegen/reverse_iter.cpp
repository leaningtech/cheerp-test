// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: Reverse iteration of static array 1/2 : SUCCESS
// CHECK: Reverse iteration of static array 2/2 : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2022 Leaning Technologies
//===----------------------------------------------------------------------===//

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

	assertEqual((int)a[0], (int)4302680, "Reverse iteration of static array 1/2");
	assertEqual((int)acc, (int)1649593728, "Reverse iteration of static array 2/2");
	return 0;
}
