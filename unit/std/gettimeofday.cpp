// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: gettimeofday 1/3 : SUCCESS
// CHECK: gettimeofday 2/3 : SUCCESS
// CHECK: gettimeofday 3/3 : SUCCESS
// CHECK: time : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2017 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>
#include <time.h>
#include <sys/time.h>

void webMain()
{
	timeval tv;
	struct timezone tz;
	int ret=gettimeofday(&tv, &tz);
	assertEqual(ret, 0, "gettimeofday 1/3");
	assertEqual(tv.tv_sec!=0, true, "gettimeofday 2/3");
	assertEqual(tv.tv_usec!=0, true, "gettimeofday 3/3");
	volatile time_t t1;
	time_t t2;
	t1=time(&t2);
	assertEqual((time_t)t1,t2, "time");
}
