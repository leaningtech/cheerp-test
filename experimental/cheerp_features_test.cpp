// RUN: mkdir -p %t
// RUN: %cheerp_clang -target cheerp-wasm -o %t/w %s
// RUN: %cheerp_clang -target cheerp -o %t/j %s
// RUN: test -f %t/w.wasm && test -f %t/w
// RUN: test -f %t/j
// RUN: wc -c %t/w.wasm | %FileCheck %s --check-prefix=WASM-SIZE
// RUN: wc -c %t/j | %FileCheck %s --check-prefix=JS-SIZE
// RUN: %node %t/w 2>&1 | %FileCheck %s --check-prefix=OUTPUT
// RUN: %node %t/j 2>&1 | %FileCheck %s --check-prefix=OUTPUT

// Test various C++ features
#include <stdio.h>

class Calculator {
public:
    int add(int a, int b) {
        return a + b;
    }
    
    int multiply(int a, int b) {
        return a * b;
    }
};

int main() {
    Calculator calc;
    
    int sum = calc.add(10, 20);
    int product = calc.multiply(4, 5);
    
    printf("Sum: %d\n", sum);
    printf("Product: %d\n", product);
    
    // Test array
    int numbers[] = {1, 2, 3, 4, 5};
    int array_sum = 0;
    for (int i = 0; i < 5; i++) {
        array_sum += numbers[i];
    }
    printf("Array sum: %d\n", array_sum);
    
    return 0;
}

// WASM-SIZE: {{[0-9]+}}
// JS-SIZE: {{[0-9]+}}
// OUTPUT: Sum: 30
// OUTPUT: Product: 20
// OUTPUT: Array sum: 15