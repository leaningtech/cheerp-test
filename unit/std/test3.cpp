// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: String empty construction 1/2 : SUCCESS
// CHECK: String empty construction 2/2 : SUCCESS
// CHECK: String construction 1/2 : SUCCESS
// CHECK: String construction 2/2 : SUCCESS
// CHECK: String insertion 1/2 : SUCCESS
// CHECK: String insertion 2/2 : SUCCESS
// CHECK: String concatenation : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2013,2016 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>
#include <string>

void webMain()
{
	std::string str;
	assertEqual(str.size(), 0u, "String empty construction 1/2");
	assertEqual(str.capacity(), 0u, "String empty construction 2/2");
	std::string str2("test");
	assertEqual(str2.size(), 4u, "String construction 1/2");
	assertEqual(str2.capacity(), 15u, "String construction 2/2");
	str2.insert(0, "pre", 3);
	assertEqual(str2, std::string("pretest"), "String insertion 1/2");
	str2.insert(0, str2.data()+3, 4);
	assertEqual(str2, std::string("testpretest"), "String insertion 2/2");
	std::string a("BBBBB-CCC-");
	a.append("AAAAAA",6);
	assertEqual(a, std::string("BBBBB-CCC-AAAAAA"), "String concatenation");
}
