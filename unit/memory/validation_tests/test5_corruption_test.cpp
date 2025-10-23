// Compilation Bug Detection Test
//
// This test verifies we can detect when compilation produces incorrect output.
// It compiles test5.cpp twice: once correctly, once with simulated compiler bug.
//
// RUN: %cheerp_clang -O1 -frtti -I%S/../.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %S/../test5.cpp -o %t_reference.js
// RUN: %node %t_reference.js 2>&1 > %t_reference.out
// RUN: %cheerp_clang -O1 -frtti -I%S/../.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %S/../test5.cpp -o %t_buggy.js
// RUN: sed -i '0,/SUCCESS/s/SUCCESS/FAILURE/' %t_buggy.js
// RUN: %node %t_buggy.js 2>&1 > %t_buggy.out
// RUN: sh -c '! diff %t_reference.out %t_buggy.out'

// TARGET: test5.cpp
// CORRUPTION: Changes first SUCCESS to FAILURE
//
// This simulates a compiler bug and verifies the test catches it.
