//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: linear-memory

// RUN: mkdir -p %t
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1 | %FileCheck %s


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

	assertPrint("ffi pointer interoperation 1/2:", Hex(wrapper_wasm(a,b)));
	// CHECK: ffi pointer interoperation 1/2: 0xffffffffffffffff
	assertPrint("ffi pointer interoperation 2/2:", Hex(wrapper_genericjs(a,b)));
	// CHECK: ffi pointer interoperation 2/2: 0xffffffffffffffff
}
