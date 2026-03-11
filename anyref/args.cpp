//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: wasm, regular
// NOTE: This test requires externref support which is WASM-only

// RUN: mkdir -p %t
// RUN: regular_only run_if_wasm compile_mode_wasm -cheerp-wasm-enable=externref -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s

// CHECK: anyref argument passing 1/2: 1
// CHECK: anyref argument passing 2/2: 1

#include "./../tests.h"
#include <vector>
#include <cheerp/client.h>

[[cheerp::genericjs]]
client::String* retValJs(const char* str)
{
	return new client::String(str);
}
[[cheerp::genericjs]]
bool argJs(client::String* s)
{
	return (new client::String("aaa") == s);
}
[[cheerp::wasm]]
[[clang::optnone]]
client::String* forward(client::String* s)
{
	return s;
}

void webMain()
{
	client::String* s = retValJs("aaa");
	bool eq = argJs(s);
	assertPrint("anyref argument passing 1/2:", eq);
	s = forward(s);
	eq = argJs(s);
	assertPrint("anyref argument passing 2/2:", eq);
}
