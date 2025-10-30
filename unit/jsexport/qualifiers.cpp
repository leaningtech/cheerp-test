// RUN: %cheerp_clang -O1 -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %cheerp_clang -O1 -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %cheerp_clang -O1 -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js

#include <cheerp/clientlib.h>

// No asserts here, just make sure it compiles.

[[cheerp::jsexport]] [[cheerp::genericjs]]
void foo(const int, const client::String&, const client::String*) {}
