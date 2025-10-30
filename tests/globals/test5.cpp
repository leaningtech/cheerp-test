//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: mkdir -p %t

// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1| %FileCheck %s
// RUN: preexec_only run_if_js compile_mode_js -o %t/j %s 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 | %FileCheck %s

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
  assertPrint("Circular dependency in global variable initializer test 1/3:", (a.next == &c) ? 1 : 0);
  // CHECK: Circular dependency in global variable initializer test 1/3: 1
  assertPrint("Circular dependency in global variable initializer test 2/3:", (b.next == &a) ? 1 : 0);
  // CHECK: Circular dependency in global variable initializer test 2/3: 1
  assertPrint("Circular dependency in global variable initializer test 3/3:", (c.next == &b) ? 1 : 0);
  // CHECK: Circular dependency in global variable initializer test 3/3: 1
}


