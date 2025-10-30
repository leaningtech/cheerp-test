// RUN: mkdir -p %t
// RUN: %cheerp_clang -target cheerp-wasm -o %t/w %s
// RUN: %cheerp_clang -target cheerp -o %t/j %s
// RUN: test -f %t/w.wasm
// RUN: test -f %t/w
// RUN: test -f %t/j
// RUN: %node %t/w 2>&1 | %FileCheck %s
// RUN: %node %t/j 2>&1 | %FileCheck %s

// Test function calls and more complex control flow
#include <stdio.h>

int fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci(n-1) + fibonacci(n-2);
}

int webMain() {
    int result = fibonacci(8);
    printf("Fibonacci(8) = %d\n", result);
    
    int sum = 0;
    for (int i = 1; i <= 5; i++) {
        sum += i;
    }
    printf("Sum 1-5 = %d\n", sum);
    
    return 0;
}

// CHECK: Fibonacci(8) = 21
// CHECK: Sum 1-5 = 15