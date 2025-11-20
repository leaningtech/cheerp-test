// Example test using .testing.js driver pattern with lit
// This demonstrates how to use the create_driver.py script to test jsexport code

// RUN: mkdir -p %t

// Regular mode - compile and run with driver
// RUN: compile_for_js -o %t/test %s
// RUN: test -f %s.testing.js && python3 %S/create_driver.py %t/test %s.testing.js %t/test_driver.js --module=vanilla || cp %t/test %t/test_driver.js
// RUN: %node %t/test_driver.js 2>&1 | %FileCheck %s --check-prefix=RUN_OK

// For ES6 module variant - note: ES6 modules create .mjs extension automatically
// RUN: compile_for_js -cheerp-make-module=es6 -o %t/test_es6.mjs %s
// RUN: test -f %s.testing.js && python3 %S/create_driver.py %t/test_es6.mjs %s.testing.js %t/test_es6_driver.mjs --module=es6 || cp %t/test_es6.mjs %t/test_es6_driver.mjs
// RUN: %node %t/test_es6_driver.mjs 2>&1 | %FileCheck %s --check-prefix=RUN_OK

// RUN_OK-NOT: unexpected exit : FAILURE
// RUN_OK-NOT: FAILURE

#include <cheerp/client.h>

class [[cheerp::jsexport]] ExampleClass {
public:
    int value;
    
    ExampleClass(int v) : value(v) {}
    
    int getValue() {
        return value;
    }
    
    int square() {
        return value * value;
    }
};
