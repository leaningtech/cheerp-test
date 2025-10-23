// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s

//===---------------------------------------------------------------------===//
//	Copyright 2024 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>
#include <cheerp/types.h>

class [[cheerp::genericjs]] [[cheerp::jsexport]] Base {};
class [[cheerp::genericjs]] [[cheerp::jsexport]] Derived : public Base {};

#define TEST(value, type) assertEqual(value->_typeof()->toUtf8(), std::string(type), "typeof " #value " == " #type)

// CHECK: typeof base == "object" : SUCCESS
// CHECK: typeof derived == "object" : SUCCESS
// CHECK: typeof primitiveString == "string" : SUCCESS
// CHECK: typeof primitiveNumber == "number" : SUCCESS
// CHECK: typeof primitiveBoolean == "boolean" : SUCCESS
// CHECK: typeof object == "object" : SUCCESS
// CHECK: typeof string == "object" : SUCCESS
// CHECK: typeof number == "object" : SUCCESS
// CHECK: typeof boolean == "object" : SUCCESS
// CHECK: typeof function == "function" : SUCCESS
// CHECK: typeof array == "object" : SUCCESS
// CHECK: typeof tarray == "object" : SUCCESS
// CHECK: typeof map == "object" : SUCCESS

[[cheerp::genericjs]]
int main() {
	client::_Any* base = new client::_Any(new Base());
	client::_Any* derived = new client::_Any(new Derived());
	client::_Any* primitiveString = new client::String("test");
	client::_Any* primitiveNumber = new client::_Any(1);
	client::_Any* primitiveBoolean;
	client::Object* object = new client::Object();
	client::Object* string = new client::Object(primitiveString);
	client::Number* number = new client::Number();
	client::Boolean* boolean = new client::Boolean();
	client::Function* function = new client::Function();
	client::Array* array = new client::Array();
	client::TArray<>* tarray = new client::TArray<>();
	client::Map<>* map = new client::Map<>();

	__asm__("true" : "=r"(primitiveBoolean));

	TEST(base, "object");
	TEST(derived, "object");
	TEST(primitiveString, "string");
	TEST(primitiveNumber, "number");
	TEST(primitiveBoolean, "boolean");
	TEST(object, "object");
	TEST(string, "object");
	TEST(number, "object");
	TEST(boolean, "object");
	TEST(function, "function");
	TEST(array, "object");
	TEST(tarray, "object");
	TEST(map, "object");

	return 1;
}
