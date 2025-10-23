// RUN: mkdir -p %t
// RUN: %cheerp_clang -target cheerp-wasm -o %t/w %s
// RUN: %cheerp_clang -target cheerp -o %t/j %s
// RUN: test -f %t/w.wasm && test -f %t/w
// RUN: test -f %t/j
// RUN: xxd -l 8 %t/w.wasm | %FileCheck %s --check-prefix=WASM-MAGIC
// RUN: cat %t/w | %FileCheck %s --check-prefix=WASM-LOADER
// RUN: cat %t/j | %FileCheck %s --check-prefix=JS-OUTPUT
// RUN: %node %t/w 2>&1 | %FileCheck %s --check-prefix=WASM-EXEC
// RUN: %node %t/j 2>&1 | %FileCheck %s --check-prefix=JS-EXEC

// Test basic arithmetic and printf functionality
#include <stdio.h>

int main() {
    int result = 8 + 12;
    printf("The result is: %d\n", result);
    return 0;
}

// WASM-MAGIC: 0061 736d
// WASM-LOADER: WebAssembly
// JS-OUTPUT: {{(printf|console)}}
// WASM-EXEC: The result is: 20
// JS-EXEC: The result is: 20