//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: mkdir -p %t

// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1| %FileCheck %s
// RUN: preexec_only run_if_js compile_mode_js -o %t/j %s 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 | %FileCheck %s

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
		assertEqual(str1, str2, "std::string::assign 1/2:");
		// CHECK: std::string::assign 1/2:
	}
	{
		std::u16string str1;
		str1.reserve(3);
		std::u16string str2;
		str2.push_back('1');
		str2.push_back('2');
		str2.push_back('5');
		str1 = str2;
		assertEqual(str1, str2, "std::string::assign 2/2:");
		// CHECK: std::string::assign 2/2:
	}
	{
		std::string str1;
		str1 = 32;
		assertEqual(str1, std::string(" "), "std::string::assign from char 1/3:");
		// CHECK: std::string::assign from char 1/3:
		str1 = "A";
		str1 = 32;

		assertEqual(str1, std::string(" "), "std::string::assign from char 2/3:");
		// CHECK: std::string::assign from char 2/3:
		str1 = "AB";
		str1 = 32;
		assertEqual(str1, std::string(" "), "std::string::assign from char 3/3:");
		// CHECK: std::string::assign from char 3/3:
	}
}
