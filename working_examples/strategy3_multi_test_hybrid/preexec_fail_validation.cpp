// RUN: mkdir -p %t
//
// Preexecution mode test (expects compilation to succeed with correct assertion):
// RUN: preexec_only compile_mode_js -o %t/test_preexec_js %s 2>&1 | %FileCheck %s --check-prefix=PREEXEC_OK
// RUN: preexec_only compile_mode_asmjs -o %t/test_preexec_asmjs %s 2>&1 | %FileCheck %s --check-prefix=PREEXEC_OK
//
// Regular mode test (expects compilation to succeed and runtime to succeed):
// RUN: regular_only compile_mode_js -o %t/test_regular_js %s 2>&1 && node %t/test_regular_js 2>&1 | %FileCheck %s --check-prefix=REG_OK
// RUN: regular_only compile_mode_asmjs -o %t/test_regular_asmjs %s 2>&1 && node %t/test_regular_asmjs 2>&1 | %FileCheck %s --check-prefix=REG_OK
// RUN: regular_only compile_mode_wasm -o %t/test_regular_wasm %s 2>&1 && node %t/test_regular_wasm 2>&1 | %FileCheck %s --check-prefix=REG_OK
//
// PREEXEC_OK-NOT: error
// REG_OK: FAILURE


#include <tests.h>

int main() {
    // In preexecution mode, compilation will succeed
    // In regular mode, the assertion will succeed at runtime
    assertEqual(2 + 3, 6, "This will fail");
    
    return 0;
}
