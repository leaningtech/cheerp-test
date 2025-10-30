// XFAIL: *
// NOTE: Requires .testing.js harness to set up argv/env for main()


//===---------------------------------------------------------------------===//
//	Copyright 2024 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp  %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs  %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm  %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: expected one argument : SUCCESS
// CHECK: expected argv to not be a nullptr : SUCCESS
// CHECK: argv.cpp : SUCCESS

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
