// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: ByVal support 1/2 : SUCCESS
// CHECK: ByVal support 2/2 : SUCCESS

#include <cheerp/client.h>
#include <tests.h>
constexpr int SIZE = 10;
struct Foo {
    int a[SIZE]{0};
    int b{0};
    [[clang::optnone]]
    void stuff(int i) {
        a[0] = i;
        for (int i = 1; i < SIZE; i++)
        {
            a[i] = a[i-1] + i;
        }
    }
};

[[clang::optnone]] void test(Foo f) {
    f.stuff(666);
}
void webMain() {
    Foo f;
    int a = unitBlackBox(10);
    f.stuff(a);
    assertEqual(f.a[0], a, "ByVal support 1/2");
    test(f);
    assertEqual(f.a[0], a, "ByVal support 2/2");
}
