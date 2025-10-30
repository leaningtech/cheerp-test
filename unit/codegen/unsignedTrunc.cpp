// XFAIL: *
// NOTE: This test requires .testing.js harness infrastructure not yet implemented in lit



// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp -cheerp-make-module=es6 %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs -cheerp-make-module=es6 %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-make-module=es6 %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s

#include <tests.h>

namespace [[cheerp::genericjs]] client
{
	bool someFunc();
}

[[cheerp::jsexport]]
int someValue(unsigned char a)
{
	unsigned char X(a);
	X+=23;

	unsigned char d;

	if (client::someFunc())
		d = (unsigned char) (X & (unsigned char)12);
	else
		d = (unsigned char)X;

	return d;
}
