//===---------------------------------------------------------------------===//
//	Copyright 2024 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: regular, js

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -cheerp-make-module=es6 -I%S/../ -o %t/env.mjs %s 2>&1
// RUN: regular_only run_if_js python3 %S/../helpers/create_driver.py %t/env.mjs %S/env.testing.js %t/env_driver.mjs --module=es6
// RUN: regular_only run_if_js %node %t/env_driver.mjs 2>&1 | %FileCheck %s

// CHECK: expected TEST environment variable to be 1: 1
// CHECK-NOT: expected environ to be passed to main: nullptr
// CHECK: expected only one environment variable: nullptr

#include "./../tests.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

extern char **environ;

int main(int argc, char **argv, char **env) {
  assertPrint("expected TEST environment variable to be 1:", getenv("TEST"));
  assertPrint("expected environ to be passed to main:", env[0]);
  assertPrint("expected only one environment variable:", env[1]);
}
