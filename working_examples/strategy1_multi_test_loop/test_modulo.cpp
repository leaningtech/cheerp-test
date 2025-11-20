// Test modulo and comparison operations across all targets
// RUN: mkdir -p %t
// RUN: %run_all_targets %s %t | %FileCheck %s --check-prefix=CHECK

#include <stdio.h>

int main() {
    int modulo = 17 % 5;
    int is_equal = (20 == 20) ? 1 : 0;
    int is_not_equal = (10 != 20) ? 1 : 0;
    
    printf("Modulo: %d\n", modulo);
    printf("Equal check: %d\n", is_equal);
    printf("Not equal check: %d\n", is_not_equal);
    
    return 0;
}

// CHECK: Modulo: 2
// CHECK: Equal check: 1
// CHECK: Not equal check: 1
