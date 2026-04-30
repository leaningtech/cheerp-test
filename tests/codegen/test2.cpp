//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: js, regular
// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>
#include <stdio.h>
#include <string.h>

void webMain()
{
	//Test printf of a very small negative float
	union {
		float f;
		unsigned int i;
	} tmp;
	tmp.i = 0xb3428184;
	char buf[20];
	sprintf(buf,"%f",tmp.f);
	
	assertPrint("Printf of very small negative float:", buf);
	//CHECK: Printf of very small negative float: -0.000000
}
