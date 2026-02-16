//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 %then_run_js | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1 | %FileCheck %s

#include "./../tests.h"
#include <cheerp/types.h>

[[cheerp::genericjs]]
int main() {
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

	assertPrint("primiteveString == client::Object: ", primitiveString->instanceof<client::Object>());
	assertPrint("primiteveString == client::String: ", primitiveString->instanceof<client::String>());
	assertPrint("primiteveString == client::Number: ", primitiveString->instanceof<client::Number>());
	assertPrint("primiteveString == client::Boolean: ", primitiveString->instanceof<client::Boolean>());
	assertPrint("primiteveString == client::Function: ", primitiveString->instanceof<client::Function>());
	assertPrint("primiteveString == client::Array: ", primitiveString->instanceof<client::Array>());
	assertPrint("primiteveString == client::TArray<>: ", primitiveString->instanceof<client::TArray<>>());
	assertPrint("primiteveString == client::Map <>: ", primitiveString->instanceof<client::Map <> >());
	
	// CHECK: primiteveString == client::Object: 0
	// CHECK: primiteveString == client::String: 0
	// CHECK: primiteveString == client::Number: 0
	// CHECK: primiteveString == client::Boolean: 0
	// CHECK: primiteveString == client::Function: 0
	// CHECK: primiteveString == client::Array: 0
	// CHECK: primiteveString == client::TArray<>: 0
	// CHECK: primiteveString == client::Map <>: 0

	assertPrint("primiteveNumber == client::Object: ", primitiveNumber->instanceof<client::Object>());
	assertPrint("primiteveNumber == client::String: ", primitiveNumber->instanceof<client::String>());
	assertPrint("primiteveNumber == client::Number: ", primitiveNumber->instanceof<client::Number>());
	assertPrint("primiteveNumber == client::Boolean: ", primitiveNumber->instanceof<client::Boolean>());
	assertPrint("primiteveNumber == client::Function: ", primitiveNumber->instanceof<client::Function>());
	assertPrint("primiteveNumber == client::Array: ", primitiveNumber->instanceof<client::Array>());
	assertPrint("primiteveNumber == client::TArray<>: ", primitiveNumber->instanceof<client::TArray<>>());
	assertPrint("primiteveNumber == client::Map <>: ", primitiveNumber->instanceof<client::Map <> >());

	// CHECK: primiteveNumber == client::Object: 0
	// CHECK: primiteveNumber == client::String: 0
	// CHECK: primiteveNumber == client::Number: 0 
	// CHECK: primiteveNumber == client::Boolean: 0
	// CHECK: primiteveNumber == client::Function: 0
	// CHECK: primiteveNumber == client::Array: 0
	// CHECK: primiteveNumber == client::TArray<>: 0
	// CHECK: primiteveNumber == client::Map <>: 0

	assertPrint("primiteveBoolean == client::Object: ", primitiveBoolean->instanceof<client::Object>());
	assertPrint("primiteveBoolean == client::String: ", primitiveBoolean->instanceof<client::String>());
	assertPrint("primiteveBoolean == client::Number: ", primitiveBoolean->instanceof<client::Number>());
	assertPrint("primiteveBoolean == client::Boolean: ", primitiveBoolean->instanceof<client::Boolean>());
	assertPrint("primiteveBoolean == client::Function: ", primitiveBoolean->instanceof<client::Function>());
	assertPrint("primiteveBoolean == client::Array: ", primitiveBoolean->instanceof<client::Array>());
	assertPrint("primiteveBoolean == client::TArray<>: ", primitiveBoolean->instanceof<client::TArray<>>());
	assertPrint("primiteveBoolean == client::Map <>: ", primitiveBoolean->instanceof<client::Map <> >());

	// CHECK: primiteveBoolean == client::Object: 0
	// CHECK: primiteveBoolean == client::String: 0
	// CHECK: primiteveBoolean == client::Number: 0
	// CHECK: primiteveBoolean == client::Boolean: 0
	// CHECK: primiteveBoolean == client::Function: 0
	// CHECK: primiteveBoolean == client::Array: 0
	// CHECK: primiteveBoolean == client::TArray<>: 0
	// CHECK: primiteveBoolean == client::Map <>: 0

	assertPrint("object == client::Object: ", object->instanceof<client::Object>());
	assertPrint("object == client::String: ", object->instanceof<client::String>());
	assertPrint("object == client::Number: ", object->instanceof<client::Number>());
	assertPrint("object == client::Boolean: ", object->instanceof<client::Boolean>());
	assertPrint("object == client::Function: ", object->instanceof<client::Function>());
	assertPrint("object == client::Array: ", object->instanceof<client::Array>());
	assertPrint("object == client::TArray<>: ", object->instanceof<client::TArray<>>());
	assertPrint("object == client::Map <>: ", object->instanceof<client::Map <> >());
	
	// CHECK: object == client::Object: 1
	// CHECK: object == client::String: 0
	// CHECK: object == client::Number: 0
	// CHECK: object == client::Boolean: 0
	// CHECK: object == client::Function: 0
	// CHECK: object == client::Array: 0
	// CHECK: object == client::TArray<>: 0
	// CHECK: object == client::Map <>: 0

	assertPrint("string == client::Object: ", string->instanceof<client::Object>());
	assertPrint("string == client::String: ", string->instanceof<client::String>());
	assertPrint("string == client::Number: ", string->instanceof<client::Number>());
	assertPrint("string == client::Boolean: ", string->instanceof<client::Boolean>());
	assertPrint("string == client::Function: ", string->instanceof<client::Function>());
	assertPrint("string == client::Array: ", string->instanceof<client::Array>());
	assertPrint("string == client::TArray<>: ", string->instanceof<client::TArray<>>());
	assertPrint("string == client::Map <>: ", string->instanceof<client::Map <> >());

	// CHECK: string == client::Object: 1
	// CHECK: string == client::String: 1
	// CHECK: string == client::Number: 0
	// CHECK: string == client::Boolean: 0
	// CHECK: string == client::Function: 0
	// CHECK: string == client::Array: 0
	// CHECK: string == client::TArray<>: 0
	// CHECK: string == client::Map <>: 0


	assertPrint("number == client::Object: ", number->instanceof<client::Object>());
	assertPrint("number == client::String: ", number->instanceof<client::String>());
	assertPrint("number == client::Number: ", number->instanceof<client::Number>());
	assertPrint("number == client::Boolean: ", number->instanceof<client::Boolean>());
	assertPrint("number == client::Function: ", number->instanceof<client::Function>());
	assertPrint("number == client::Array: ", number->instanceof<client::Array>());
	assertPrint("number == client::TArray<>: ", number->instanceof<client::TArray<>>());
	assertPrint("number == client::Map <>: ", number->instanceof<client::Map <> >());

	// CHECK: number == client::Object: 1
	// CHECK: number == client::String: 0
	// CHECK: number == client::Number: 1
	// CHECK: number == client::Boolean: 0
	// CHECK: number == client::Function: 0
	// CHECK: number == client::Array: 0
	// CHECK: number == client::TArray<>: 0
	// CHECK: number == client::Map <>: 0

	assertPrint("boolean == client::Object: ", boolean->instanceof<client::Object>());
	assertPrint("boolean == client::String: ", boolean->instanceof<client::String>());
	assertPrint("boolean == client::Number: ", boolean->instanceof<client::Number>());
	assertPrint("boolean == client::Boolean: ", boolean->instanceof<client::Boolean>());
	assertPrint("boolean == client::Function: ", boolean->instanceof<client::Function>());
	assertPrint("boolean == client::Array: ", boolean->instanceof<client::Array>());
	assertPrint("boolean == client::TArray<>: ", boolean->instanceof<client::TArray<>>());
	assertPrint("boolean == client::Map <>: ", boolean->instanceof<client::Map <> >());

	// CHECK: boolean == client::Object: 1
	// CHECK: boolean == client::String: 0
	// CHECK: boolean == client::Number: 0
	// CHECK: boolean == client::Boolean: 1
	// CHECK: boolean == client::Function: 0
	// CHECK: boolean == client::Array: 0
	// CHECK: boolean == client::TArray<>: 0
	// CHECK: boolean == client::Map <>: 0

	assertPrint("function == client::Object: ", function->instanceof<client::Object>());
	assertPrint("function == client::String: ", function->instanceof<client::String>());
	assertPrint("function == client::Number: ", function->instanceof<client::Number>());
	assertPrint("function == client::Boolean: ", function->instanceof<client::Boolean>());
	assertPrint("function == client::Function: ", function->instanceof<client::Function>());
	assertPrint("function == client::Array: ", function->instanceof<client::Array>());
	assertPrint("function == client::TArray<>: ", function->instanceof<client::TArray<>>());
	assertPrint("function == client::Map <>: ", function->instanceof<client::Map <> >());

	// CHECK: function == client::Object: 1
	// CHECK: function == client::String: 0
	// CHECK: function == client::Number: 0
	// CHECK: function == client::Boolean: 0
	// CHECK: function == client::Function: 1
	// CHECK: function == client::Array: 0
	// CHECK: function == client::TArray<>: 0
	// CHECK: function == client::Map <>: 0

	assertPrint("array == client::Object: ", array->instanceof<client::Object>());
	assertPrint("array == client::String: ", array->instanceof<client::String>());
	assertPrint("array == client::Number: ", array->instanceof<client::Number>());
	assertPrint("array == client::Boolean: ", array->instanceof<client::Boolean>());
	assertPrint("array == client::Function: ", array->instanceof<client::Function>());
	assertPrint("array == client::Array: ", array->instanceof<client::Array>());
	assertPrint("array == client::TArray<>: ", array->instanceof<client::TArray<>>());
	assertPrint("array == client::Map <>: ", array->instanceof<client::Map <> >());

	// CHECK: array == client::Object: 1
	// CHECK: array == client::String: 0
	// CHECK: array == client::Number: 0
	// CHECK: array == client::Boolean: 0
	// CHECK: array == client::Function: 0
	// CHECK: array == client::Array: 1
	// CHECK: array == client::TArray<>: 1
	// CHECK: array == client::Map <>: 0


	assertPrint("tarray == client::Object: ", tarray->instanceof<client::Object>());
	assertPrint("tarray == client::String: ", tarray->instanceof<client::String>());
	assertPrint("tarray == client::Number: ", tarray->instanceof<client::Number>());
	assertPrint("tarray == client::Boolean: ", tarray->instanceof<client::Boolean>());
	assertPrint("tarray == client::Function: ", tarray->instanceof<client::Function>());
	assertPrint("tarray == client::Array: ", tarray->instanceof<client::Array>());
	assertPrint("tarray == client::TArray<>: ", tarray->instanceof<client::TArray<>>());
	assertPrint("tarray == client::Map <>: ", tarray->instanceof<client::Map <> >());

	// CHECK: tarray == client::Object: 1
	// CHECK: tarray == client::String: 0
	// CHECK: tarray == client::Number: 0
	// CHECK: tarray == client::Boolean: 0
	// CHECK: tarray == client::Function: 0
	// CHECK: tarray == client::Array: 1
	// CHECK: tarray == client::TArray<>: 1
	// CHECK: tarray == client::Map <>: 0

	assertPrint("map == client::Object: ", map->instanceof<client::Object>());
	assertPrint("map == client::String: ", map->instanceof<client::String>());
	assertPrint("map == client::Number: ", map->instanceof<client::Number>());
	assertPrint("map == client::Boolean: ", map->instanceof<client::Boolean>());
	assertPrint("map == client::Function: ", map->instanceof<client::Function>());
	assertPrint("map == client::Array: ", map->instanceof<client::Array>());
	assertPrint("map == client::TArray<>: ", map->instanceof<client::TArray<>>());
	assertPrint("map == client::Map <>: ", map->instanceof<client::Map <> >());

	// CHECK: map == client::Object: 1
	// CHECK: map == client::String: 0
	// CHECK: map == client::Number: 0
	// CHECK: map == client::Boolean: 0
	// CHECK: map == client::Function: 0
	// CHECK: map == client::Array: 0
	// CHECK: map == client::TArray<>: 0
	// CHECK: map == client::Map <>: 1

	return 1;
}
