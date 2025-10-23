// Compilation Bug Detection Test
//
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %S/test1.cpp -o %t_reference.js
// RUN: %node %t_reference.js 2>&1 > %t_reference.out
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %S/test1.cpp -o %t_buggy.js
// RUN: sed -i '0,/SUCCESS/s/SUCCESS/FAILURE/' %t_buggy.js
// RUN: %node %t_buggy.js 2>&1 > %t_buggy.out
// RUN: sh -c '! diff %t_reference.out %t_buggy.out'

// TARGET: test1.cpp (memmove tests)
// CORRUPTION: Changes first occurrence of SUCCESS to FAILURE in the JS output
//
// This simulates a compiler bug and verifies the test catches it.
