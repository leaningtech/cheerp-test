//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


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


