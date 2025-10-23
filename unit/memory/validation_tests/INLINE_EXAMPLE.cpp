// Example: test1.cpp with built-in validation tests
//
// This shows how you COULD combine everything into one file

// ============================================
// ORIGINAL FUNCTIONALITY TESTS
// ============================================
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s

// ============================================
// VALIDATION TEST: BUGLESS (Baseline)
// ============================================
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t_validation_ref.js
// RUN: %node %t_validation_ref.js 2>&1 > %t_validation_ref.out
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t_validation_test.js
// RUN: %node %t_validation_test.js 2>&1 > %t_validation_test.out
// RUN: diff %t_validation_ref.out %t_validation_test.out

// ============================================
// VALIDATION TEST: CORRUPTION (Bug Detection)
// ============================================
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t_corruption_ref.js
// RUN: %node %t_corruption_ref.js 2>&1 > %t_corruption_ref.out
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t_corruption_buggy.js
// RUN: sed -i '0,/SUCCESS/s/SUCCESS/FAILURE/' %t_corruption_buggy.js
// RUN: %node %t_corruption_buggy.js 2>&1 > %t_corruption_buggy.out
// RUN: sh -c '! diff %t_corruption_ref.out %t_corruption_buggy.out'

// CHECK: Memmove, (native types) on isolated memory : SUCCESS
// CHECK: Memmove, (native types) destination after source : SUCCESS
// ... etc

// [Rest of test code here]
