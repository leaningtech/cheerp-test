//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 %then_run_js | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1 | %FileCheck %s

#include <tests.h>
#include <cheerp/types.h>
#include <stdio.h>
#include <iostream>

class [[cheerp::genericjs]] [[cheerp::jsexport]] Base {};
class [[cheerp::genericjs]] [[cheerp::jsexport]] Derived : public Base {};

#define TEST(value, type) assertEqual(value->_typeof()->toUtf8(), std::string(type), "typeof " #value " == " #type)

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

	assertPrint("typeof base: ", (base->_typeof()->toUtf8() == "object"));
	assertPrint("typeof derived: ", (derived->_typeof()->toUtf8() == "object"));
	assertPrint("typeof primitiveString: ", (primitiveString->_typeof()->toUtf8() == "string"));
	assertPrint("typeof primitiveNumber: ", (primitiveNumber->_typeof()->toUtf8() == "number"));
	assertPrint("typeof primitiveBoolean: ", (primitiveBoolean->_typeof()->toUtf8() == "boolean"));
	assertPrint("typeof object: ", (object->_typeof()->toUtf8()) == "object");
	assertPrint("typeof string: ", (string->_typeof()->toUtf8()) == "object");
	assertPrint("typeof number: ", (number->_typeof()->toUtf8()) == "object");
	assertPrint("typeof boolean: ", (boolean->_typeof()->toUtf8()) == "object");
	assertPrint("typeof function: ", (function->_typeof()->toUtf8() == "function"));
	assertPrint("typeof array: ", (array->_typeof()->toUtf8()) == "object");
	assertPrint("typeof tarray: ", (tarray->_typeof()->toUtf8() == "object"));
	assertPrint("typeof map: ", (map->_typeof()->toUtf8()) == "object");

	// CHECK: typeof base: 1
	// CHECK: typeof derived: 1
	// CHECK: typeof primitiveString: 1 
	// CHECK: typeof primitiveNumber: 1 
	// CHECK: typeof primitiveBoolean: 1
	// CHECK: typeof object: 1
	// CHECK: typeof string: 1
	// CHECK: typeof number: 1
	// CHECK: typeof boolean: 1
	// CHECK: typeof function: 1
	// CHECK: typeof array: 1
	// CHECK: typeof tarray: 1
	// CHECK: typeof map: 1

	return 1;
}
