//===---------------------------------------------------------------------===//
//	Copyright 2020 Leaning Technlogies
//===----------------------------------------------------------------------===//

// REQUIRES: wasm

// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs -cheerp-use-bigints %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-use-bigints %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: ffi pointer interoperation 1/2 : SUCCESS
// CHECK: ffi pointer interoperation 2/2 : SUCCESS

#include <tests.h>

using u64 = unsigned long long;

[[cheerp::wasm]]
u64 process_wasm(u64 a, u64 b) {
	return a+b;
}
[[cheerp::genericjs]]
u64 wrapper_genericjs(u64 a, u64 b) {
	return process_wasm(a,b);
}
[[cheerp::genericjs]]
u64 process_genericjs(u64 a, u64 b) {
	return a+b;
}
[[cheerp::wasm]]
u64 wrapper_wasm(u64 a, u64 b) {
	return process_genericjs(a,b);
}

void webMain()
{
	u64 a = unitBlackBox<u64>(0x0f0f0f0f0f0f0f0f);
	u64 b = unitBlackBox<u64>(0xf0f0f0f0f0f0f0f0);
	assertEqual(wrapper_wasm(a,b), unitBlackBox<u64>(0xffffffffffffffff), "ffi pointer interoperation 1/2");
	assertEqual(wrapper_genericjs(a,b), unitBlackBox<u64>(0xffffffffffffffff), "ffi pointer interoperation 2/2");
}
