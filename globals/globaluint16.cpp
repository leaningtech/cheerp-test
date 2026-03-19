//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t

// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1| %FileCheck %s
// RUN: preexec_only run_if_js compile_mode_js -o %t/j %s 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 | %FileCheck %s

#include <tests.h>
#include <stdint.h>
#include <stdio.h>

uint16_t global = 123;

void doSomething()
{
	char buffer[100];
	sprintf(buffer, "%d\n", global);

	int len = strlen(buffer);
	assertPrint("Global uint16_t length check:", (len < 7) ? 1 : 0);
}

int main()
{
	for (int i=0; i<53; i++)
	{
		global *= global;
		doSomething();
	}
	return global;
}

// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
// CHECK: Global uint16_t length check: 1
