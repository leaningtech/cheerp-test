// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: std::string::assign : SUCCESS
// CHECK: std::u16string::assign : SUCCESS
// CHECK: std::string::assign from char 1/3 : SUCCESS
// CHECK: std::string::assign from char 2/3 : SUCCESS
// CHECK: std::string::assign from char 3/3 : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2017 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>
#include <string>

void webMain()
{
	{
		std::string str1;
		str1.reserve(3);
		std::string str2;
		str2.push_back('1');
		str2.push_back('2');
		str2.push_back('5');
		str1 = str2;
		assertEqual(str1, str2, "std::string::assign");
	}
	{
		std::u16string str1;
		str1.reserve(3);
		std::u16string str2;
		str2.push_back('1');
		str2.push_back('2');
		str2.push_back('5');
		str1 = str2;
		assertEqual(str1, str2, "std::u16string::assign");
	}
	{
		std::string str1;
		str1 = 32;
		assertEqual(str1, std::string(" "), "std::string::assign from char 1/3");
		str1 = "A";
		str1 = 32;
		assertEqual(str1, std::string(" "), "std::string::assign from char 2/3");
		str1 = "AB";
		str1 = 32;
		assertEqual(str1, std::string(" "), "std::string::assign from char 3/3");
	}
}
