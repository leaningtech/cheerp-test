// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s

//===---------------------------------------------------------------------===//
//	Copyright 2013 Leaning Technlogies
//===----------------------------------------------------------------------===//

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
	
	assertEqual(strcmp(buf,"-0.000000"), 0, "Printf of very small negative float");
}
