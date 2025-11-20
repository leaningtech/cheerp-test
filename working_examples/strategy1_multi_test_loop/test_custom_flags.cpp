// Test to verify custom compilation flags
// RUN: mkdir -p %t
// RUN: %run_all_targets %s %t 2>&1 | %FileCheck %s
// CHECK: Custom flags test

#include <stdio.h>

int main() {
#ifdef NDEBUG
    printf("Custom flags test: NDEBUG defined\n");
#else
    printf("Custom flags test: Debug mode\n");
#endif
    return 0;
}
