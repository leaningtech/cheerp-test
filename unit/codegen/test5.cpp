// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: Memmove backward 1/4 : SUCCESS
// CHECK: Memmove backward 2/4 : SUCCESS
// CHECK: Memmove backward 3/4 : SUCCESS
// CHECK: Memmove backward 4/4 : SUCCESS

//===---------------------------------------------------------------------===//
//      Copyright 2014 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>

void webMain()
{
	int v[] = {0,1,2,3};
	memmove(v + 1, v, 3*sizeof(int));

	assertEqual(v[0],0,"Memmove backward 1/4");
	assertEqual(v[1],0,"Memmove backward 2/4");
	assertEqual(v[2],1,"Memmove backward 3/4");
	assertEqual(v[3],2,"Memmove backward 4/4");
}
