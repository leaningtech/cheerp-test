//===---------------------------------------------------------------------===//
//	Copyright 2020 Leaning Technlogies
//===----------------------------------------------------------------------===//

// REQUIRES: wasm

// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-wasm-enable=externref %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: anyref argument passing 1/2 : SUCCESS
// CHECK: anyref argument passing 2/2 : SUCCESS

#include <tests.h>
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
	assertEqual(eq, true, "anyref argument passing 1/2");
	s = forward(s);
	eq = argJs(s);
	assertEqual(eq, true, "anyref argument passing 2/2");
}
