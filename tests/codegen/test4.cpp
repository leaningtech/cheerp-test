//===---------------------------------------------------------------------===//
//      Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s

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
