//===---------------------------------------------------------------------===//
//	Copyright 2013 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t
// Preexecution mode (PREEXECUTE_MODE=true):
// RUN: preexec_only run_if_js %valgrind compile_mode_js -o %t/j_pre %s 2>&1 | %FileCheck %s 
// RUN: preexec_only run_if_asmjs %valgrind compile_mode_asmjs -o %t/a_pre %s 2>&1 | %FileCheck %s

// Regular mode (PREEXECUTE_MODE=false):
// RUN: regular_only run_if_wasm %valgrind compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s 
// RUN: regular_only run_if_js %valgrind compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s 
// RUN: regular_only run_if_asmjs %valgrind compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1 | %FileCheck %s 

// CHECK: Integer test: 42
// CHECK: Double test: 3.14159
// CHECK: Float test: 2.71828
// CHECK: Size_t test: 12345
// CHECK: String test: Hello World
// CHECK: Negative int: -99
// CHECK: Large int: 2147483647
// CHECK: Zero: 0
// CHECK: Small double: 0.000000001
// CHECK: multiple vals: 42 2.7182800770 0.000000001

#include "tests.h"

void webMain()
{
	int intVal = 42;
	__preexecute_print_case("Integer test: ", intVal);
	
	double doubleVal = 3.14159265358979;
	__preexecute_print_case("Double test: ", doubleVal);

	float floatVal = 2.71828f;
	__preexecute_print_case("Float test: ", floatVal);
	
	size_t sizeVal = 12345;
	__preexecute_print_case("Size_t test: ", sizeVal);
	
	const char* strVal = "Hello World";
	__preexecute_print_case("String test: ", strVal);
	
	int negVal = -99;
	__preexecute_print_case("Negative int: ", negVal);
	
	int largeVal = 2147483647;  // INT_MAX
	__preexecute_print_case("Large int: ", largeVal);
	
	int zeroVal = 0;
	__preexecute_print_case("Zero: ", zeroVal);
	
	double smallDouble = 0.0000000010;
	__preexecute_print_case("Small double: ", smallDouble);

	__preexecute_print_case("multiple vals: ", intVal, floatVal, smallDouble);
}
