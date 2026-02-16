//===---------------------------------------------------------------------===//
//      Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_js compile_mode_js -o %t/j %s 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 | %FileCheck %s

#include <tests.h>

int * foo(int * a,int * b)
{
    a[2] = b[1];
    b[2] = a[1];
 
    return a[1] < b[1] ? a + 1 : b+1;
}
 
void webMain()
{
    int A[] = {1,2,3};
    int B[] = {7,5,4};
 
    int * C = foo(A,B);
    
    //A == {1,2,5}
    //B == {7,5,2}
    //C == &A[1]
 
    //TODO put intrinsic to assert the correct pointer type
    if (C == &A[1])
        assertPrint("Pointer equality comparison:");
    assertPrint("Pointer equality comparison:", A[2]);
    assertPrint("Pointer equality comparison:", B[2]);

    //CHECK: Pointer equality comparison:
    //CHECK: Pointer equality comparison: 5
    //CHECK: Pointer equality comparison: 2
}
