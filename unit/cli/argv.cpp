//===---------------------------------------------------------------------===//
//	Copyright 2024 Leaning Technologies
//===----------------------------------------------------------------------===//

// This test uses a wrapper to set up argv for ES6 module execution
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp -cheerp-make-module=es6 %s -o %t.mjs
// RUN: %S/../../test-wrappers/cli-argv-wrapper.mjs %t.mjs 2>&1 | %FileCheck %s
// CHECK: expected one argument : SUCCESS
// CHECK: expected argv to not be a nullptr : SUCCESS
// CHECK: expected argument 0 to be the filename : SUCCESS

#include <tests.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

extern char **environ;

int main(int argc, char **argv) {
  assertEqual(argc, 1, "expected one argument");
  assertEqual(argv!=nullptr, true, "expected argv to not be a nullptr");
  assertEqual(strcmp(argv[0], "argv.cpp"), 0, "expected argument 0 to be the filename");
}
