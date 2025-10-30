//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: mkdir -p %t

// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck --check-prefixes=CHECK-REG,CHECK %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck --check-prefixes=CHECK-REG,CHECK %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1| %FileCheck --check-prefixes=CHECK-REG,CHECK %s
// RUN: preexec_only run_if_js compile_mode_js -o %t/j %s 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 | %FileCheck %s

#include <tests.h>
#include <iostream>
#include <sstream>

void webMain()
{
#ifndef PRE_EXECUTE_TEST
	//iostream and sstream test
	std::cout << "Cout output : SUCCESS" << std::endl;
	std::cerr << "Cerr output : SUCCESS" << std::endl;
	// CHECK-REG: Cout output : SUCCESS
	// CHECK-REG: Cerr output : SUCCESS
#endif
	std::ostringstream str;
	str << "test " << 42;
	// assertEqual(str.str().c_str(), "test 42", "stringstream 1/2");
	assertPrint("stringstream 1/2:", str.str().c_str());
	// CHECK: stringstream 1/2: test 42
	// Input/Output string stream
	std::stringstream str2;
	str2 << "43";
	int res;
	str2 >> res;
	// assertEqual(res, 43, "stringstream 2/2");
	assertPrint("stringstream 2/2:", res);
	// CHECK: stringstream 2/2: 43
}
