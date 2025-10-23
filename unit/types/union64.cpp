// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s

//===---------------------------------------------------------------------===//
//	Copyright 2019 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>
#include <math.h>

union U
{
	double d;
	uint64_t u64;
};

void webMain()
{
	//Test union with uint64_t
	U u;
	u.d = M_PI;
	assertEqual<uint64_t>(u.u64, 4614256656552045848, "Access to union with unt64_t member 1/2");
	u.d = 0.0;
	u.u64 = 4614256656552045848;
	assertEqual<double>(u.d, M_PI, "Access to union with unt64_t member 2/2");
}
