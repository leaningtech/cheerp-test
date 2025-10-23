// Compilation Bug Detection Test
//
// This test verifies we can detect when compilation produces incorrect output.
//
// TO ADAPT FOR OTHER TEST FILES:
// 1. Copy: cp multiarray_corruption_test.cpp <testname>_corruption_test.cpp
// 2. Change line 10 & 12: Replace "multiarray.cpp" with your test file
// 3. Change line 13: Adjust sed pattern to corrupt test-specific values
//    Examples:
//      - For numeric tests: 's/66/65/g', 's/100/99/g', 's/42/41/g'
//      - For array sizes: 's/\[12\]/[11]/g', 's/\*4\*/\*3\*/g'
//      - For counters: 's/24/23/g', 's/count+1/count/g'
//      - For output strings: '0,/SUCCESS/s/SUCCESS/FAILURE/' (changes first SUCCESS)
//
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %S/multiarray.cpp -o %t_reference.js
// RUN: %node %t_reference.js 2>&1 > %t_reference.out
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %S/multiarray.cpp -o %t_buggy.js
// RUN: sed -i 's/66/65/g' %t_buggy.js
// RUN: %node %t_buggy.js 2>&1 > %t_buggy.out
// RUN: sh -c '! diff %t_reference.out %t_buggy.out'

// TARGET: multiarray.cpp  
// CORRUPTION: Changes expected value 66 to 65 (array sum test)
//
// This simulates a compiler bug and verifies the test catches it.
