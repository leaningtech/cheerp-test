// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: Circular dependency in global variable initializer : SUCCESS
// CHECK: Circular dependency in global variable initializer : SUCCESS
// CHECK: Circular dependency in global variable initializer : SUCCESS

//===---------------------------------------------------------------------===//
//      Copyright 2014 Leaning Technlogies
//===----------------------------------------------------------------------===//

// Test circular dependency between global variables initializers.

#include <tests.h>

struct foo
{
  foo * next;
};

extern foo c;

foo a = { &c };
foo b = { &a };
foo c = { &b };


void webMain()
{
 assertEqual(a.next,&c,"Circular dependency in global variable initializer");
 assertEqual(b.next,&a,"Circular dependency in global variable initializer");
 assertEqual(c.next,&b,"Circular dependency in global variable initializer");
}



