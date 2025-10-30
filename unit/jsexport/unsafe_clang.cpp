// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s

#include <stdint.h>
#include <tests.h>

[[cheerp::jsexport_unsafe]]
uint32_t doSomething2(uint32_t a)
{
	return a*a;
}

int main()
{
	uint32_t val = 5;
	uint32_t result = doSomething2(val);
	assertEqual<uint32_t>(result, val * val, "Unsafe_clang executed");
	return 0;
}

//CHECK: Unsafe_clang executed : SUCCESS