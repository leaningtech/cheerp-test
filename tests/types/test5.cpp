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

void webMain()
{
	//Test small integer math
	volatile unsigned short a=10;
	volatile unsigned short b=0xfffe;
	assertPrint("Int16 unsigned addition:", (unsigned short)(b + a));
	// CHECK: Int16 unsigned addition: 8
	assertPrint("Int16 unsigned subtraction:", (Hex((unsigned short)(b - a))));
	// CHECK: Int16 unsigned subtraction: 0xfff4
	assertPrint("Int16 unsigned multiplication:", Hex((unsigned short)(b * a)));
	// CHECK: Int16 unsigned multiplication: 0xffec
	assertPrint("Int16 unsigned division:", Hex((unsigned short)(b / a)));
	// CHECK: Int16 unsigned division: 0x1999
	assertPrint("Int16 unsigned ramainder:", Hex((unsigned short)(b % a)));

	volatile signed short c=10;
	volatile signed short d=0xfffe;
	assertPrint("Int16 signed addition:", d + c);
	// CHECK: Int16 signed addition: 8
	assertPrint("Int16 signed subtraction:", d - c); 
	// CHECK: Int16 signed subtraction: -12
	assertPrint("Int16 signed multiplication:", d * c);
	// CHECK: Int16 signed multiplication: -20
	assertPrint("Int16 signed multiplication:", c / d);
	// CHECK: Int16 signed multiplication: -5
	assertPrint("Int16 signed remainder:", d % c);
	// CHECK: Int16 signed remainder: -2
}

