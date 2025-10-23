// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: stringstream 1/2 : SUCCESS
// CHECK: stringstream 2/2 : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2013 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>
#include <iostream>
#include <sstream>

void webMain()
{
#ifndef PRE_EXECUTE_TEST
	//iostream and sstream test
	std::cout << "Cout output : SUCCESS" << std::endl;
	std::cerr << "Cerr output : SUCCESS" << std::endl;
#endif
	std::ostringstream str;
	str << "test " << 42;
	assertEqual(str.str().c_str(), "test 42", "stringstream 1/2");
	// Input/Output string stream
	std::stringstream str2;
	str2 << "43";
	int res;
	str2 >> res;
	assertEqual(res, 43, "stringstream 2/2");
}
