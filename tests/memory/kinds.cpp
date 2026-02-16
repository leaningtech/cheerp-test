//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: mkdir -p %t

// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s

#include <tests.h>

void webMain()
{
	//Check pointer kinds assigned by Cheerp
	client::String* str = new client::String();
	assertPrint("Pointer kind on client object:", (__builtin_cheerp_pointer_kind(str) == 0u)); //COMPLETE_OBJECT = 0u
	// CHECK: Pointer kind on client object: 1

	std::string* str2 = new std::string("text");
	assertPrint("Pointer kind on heap allocated object:", (__builtin_cheerp_pointer_kind(str2) == 0u)); //COMPLETE_OBJECT = 0u
	// CHECK: Pointer kind on heap allocated object: 1
	delete str2;

	union u
	{
		int i;
		float f;
	};

	u* blData = new u();
	blData->i = 0x41400000;
	assertPrint("Pointer kind on union 1/2:", blData->f);
	// CHECK: Pointer kind on union 1/2: 12.000000000000000000
	assertPrint("Pointer kind on union 2/2:", (__builtin_cheerp_pointer_kind(blData) == 3u)); // BYTE_LAYOUT = 3u
	// CHECK: Pointer kind on union 2/2: 1
	delete blData;
	
	int a[] = {1,2,3,4,5,6,7,8};

	int* ap = a+2;

	assertPrint("Pointer kind on array:", (__builtin_cheerp_pointer_kind(ap) == 1u)); // SPLIT_REGULAR = 1u
	// CHECK: Pointer kind on array: 1
	assertPrint("Force array loads:", *ap); 
	// CHECK: Force array loads: 3
}
