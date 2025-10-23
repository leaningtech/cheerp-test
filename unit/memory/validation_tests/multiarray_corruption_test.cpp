// Compilation Bug Detection Test
//
// This test verifies we can detect when compilation produces incorrect output.
// It compiles multiarray.cpp twice: once correctly, once with simulated compiler bug.
//
// RUN: %cheerp_clang -O1 -frtti -I%S/../.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %S/../multiarray.cpp -o %t_reference.js
// RUN: %node %t_reference.js 2>&1 > %t_reference.out
// RUN: %cheerp_clang -O1 -frtti -I%S/../.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %S/../multiarray.cpp -o %t_buggy.js
// RUN: sed -i 's/\b66\b/65/g' %t_buggy.js
// RUN: %node %t_buggy.js 2>&1 > %t_buggy.out
// RUN: sh -c '! diff %t_reference.out %t_buggy.out'

// TARGET: multiarray.cpp
// CORRUPTION: Changes numeric value 66 to 65
//
// This simulates a compiler bug and verifies the test catches it.
