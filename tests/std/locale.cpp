//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>
#include <clocale>

void webMain()
{
	const char* loc;
	loc = setlocale(LC_ALL,"C");
	assertPrint("setlocale 1/4:", loc);
	// CHECK: setlocale 1/4: C

	loc = setlocale(LC_ALL,"C.UTF-8");
	assertPrint("setlocale 2/4:", loc);
	// CHECK: setlocale 2/4: C.UTF-8;C;C;C;C;C

	// Go back to default ("C.UTF-8")
	loc = setlocale(LC_ALL,"");
	assertPrint("setlocale 3/4:", loc);
	// CHECK: setlocale 3/4: C.UTF-8;C;C;C;C;C

	// Invalid locale
	loc = setlocale(LC_ALL,"GARBAGE");
	assertPrint("setlocale 4/4:", loc);
	// CHECK: setlocale 4/4: GARBAGE
}
