//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// REQUIRES: regular 

// RUN: mkdir -p %t

// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1| %FileCheck %s

#include <tests.h>
#include <time.h>
#include <sys/time.h>

void webMain()
{
	timeval tv;
	struct timezone tz;
	int ret=gettimeofday(&tv, &tz);
	assertPrint("gettimeofday 1/3:", ret);
	// CHECK: gettimeofday 1/3: 0
	assertPrint("gettimeofday 2/3:", tv.tv_sec != 0);
	// CHECK: gettimeofday 2/3: 1
	assertPrint("gettimeofday 3/3:", tv.tv_usec != 0);
	// CHECK: gettimeofday 3/3: 1
	volatile time_t t1;
	time_t t2;
	t1=time(&t2);
	assertPrint("time:", (time_t)t1 - t2);
	// CHECK: time: 0
}
