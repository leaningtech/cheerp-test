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
	std::string str;
	assertPrint("String empty construction 1/2:", str.size());
	// CHECK: String empty construction 1/2: 0
	assertPrint("String empty construction 2/2:", str.capacity());
	std::string str2("test");
	assertPrint("String construction 1/2:", str2.size());
	// CHECK: String construction 1/2: 4
	assertPrint("String construction 2/2:", str2.capacity());
	// CHECK: String construction 2/2: 15
	str2.insert(0, "pre", 3);
	assertPrint("String insertion 1/2:", str2.c_str());
	// CHECK: String insertion 1/2: pretest
	str2.insert(0, str2.data()+3, 4);
	assertPrint("String insertion 2/2:", str2.c_str());
	// CHECK: String insertion 2/2: testpretest
	std::string a("BBBBB-CCC-");
	a.append("AAAAAA",6);
	assertPrint("String concatenation:", a.c_str());
	// CHECK: String concatenation: BBBBB-CCC-AAAAAA
}
