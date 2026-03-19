//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: regular, js

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -cheerp-make-module=es6 -o %t/argv.mjs %s 2>&1
// RUN: regular_only run_if_js python3 %S/../helpers/create_driver.py %t/argv.mjs %S/argv.testing.js %t/argv_driver.mjs --module=es6
// RUN: regular_only run_if_js %node %t/argv_driver.mjs 2>&1 | %FileCheck %s

// CHECK: argc: 1
// CHECK-NOT: value of argv: nullptr
// CHECK: value of argv[0]: argv.cpp

#include "./../tests.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <cheerp/client.h>
extern char **environ;

int main(int argc, char **argv) {
  assertPrint("argc:", argc);
  assertPrint("value of argv:", argv);
  assertPrint("value of argv[0]: ", argv[0]);
}
