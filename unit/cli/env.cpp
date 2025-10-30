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
// CHECK: TEST : SUCCESS
// CHECK: TEST=1 : SUCCESS
// CHECK: expected only one environment variable : SUCCESS

#include <tests.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

extern char **environ;

int main(int argc, char **argv, char **env) {
  assertEqual(strcmp(getenv("TEST"), "1")==0, true, "expected TEST environment variable to be 1");
  assertEqual(strcmp(env[0], "TEST=1")==0, true, "expected environ to be passed to main");
  assertEqual(env[1]==nullptr, true, "expected only one environment variable");
}
