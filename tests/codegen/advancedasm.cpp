//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: regular, js

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 %then_run_js | %FileCheck %s

#include <cheerp/client.h>
#include <tests.h>

namespace client {

class IntString: public Object {
public:
	int get_field1();
	client::String* get_field2();
};

}

void webMain()
{
	int a;
	client::String* s = new client::String("testString");
	__asm__("1+%1.length" : "=r"(a) : "r"(s));
	assertPrint("Advanced inline asm 1/3:", a);
	// CHECK: Advanced inline asm 1/3: 11
	double b;
	__asm__("0.5+%1.length" : "=r"(b) : "r"(s));
	assertPrint("Advanced inline asm 2/3:", b);
	// CHECK: Advanced inline asm 2/3: 10.5
	// assertEqual(b, 10.5, "Advanced inline asm 2/3");
	client::String* s2;
	__asm__("%1" : "=r"(s2) : "r"(s));
	assertPrint("Advanced inline asm 3/3:", (s2 == s));
	// CHECK: Advanced inline asm 3/3: 1
	// assertEqual(s2, s, "Advanced inline asm 3/3");
	// Test clobbering
	__asm__("var f=%0" : : "r"(3) : "f","g");

	// Test CHEERP_OBJECT macro
	int field1 = 1;
	client::String* field2 = new client::String("hello");
	client::IntString* is = static_cast<client::IntString*>(CHEERP_OBJECT(field1, field2));
	assertPrint("CHEERP_OBJECT macro 1/2:", (is->get_field1() == field1));
	// CHECK: CHEERP_OBJECT macro 1/2: 1
	assertPrint("CHEERP_OBJECT macro 2/2:", (is->get_field2() == field2));
	// CHECK: CHEERP_OBJECT macro 2/2: 1
	// assertEqual(is->get_field1(), field1, "CHEERP_OBJECT macro 1/2");
	// assertEqual(is->get_field2(), field2, "CHEERP_OBJECT macro 2/2");
}
