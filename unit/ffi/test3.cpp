// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: Static constructors 1/2 : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2019 Leaning Technlogies
//===---------------------------------------------------------------------===//

#include <tests.h>
#include <cheerp/client.h>
#include <string>
#include <iostream>

#include <cstddef>

[[cheerp::genericjs]] client::String* s = new client::String("aaa");
std::string ss = "bbb";

int main()
{
	assertEqual(ss, std::string("bbb"), "Static constructors 1/2");
	return 1;
}

[[cheerp::genericjs]] void webMain()
{
	main();
	client::String* i = new client::String("aaa");
	assertEqualImpl(s == i, "Static constructor 2/2");
}
