// Bugless Test - Baseline Verification
//
// This test verifies that identical compilations produce identical output.
// It compiles test2.cpp twice without corruption and expects the outputs to match.
//
// RUN: %cheerp_clang -O1 -frtti -I%S/../.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %S/../test2.cpp -o %t_reference.js
// RUN: %node %t_reference.js 2>&1 > %t_reference.out
// RUN: %cheerp_clang -O1 -frtti -I%S/../.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %S/../test2.cpp -o %t_buggy.js
// RUN: %node %t_buggy.js 2>&1 > %t_buggy.out
// RUN: diff %t_reference.out %t_buggy.out

// TARGET: test2.cpp
// NO CORRUPTION - This is the baseline test
//
// This verifies that without corruption, outputs are identical (test should PASS).
