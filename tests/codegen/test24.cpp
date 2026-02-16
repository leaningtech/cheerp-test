//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_js compile_mode_js -o %t/j %s 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 | %FileCheck %s

#include <tests.h>

void webMain()
{
	std::string buffer;
	int val = 100;
	uint64_t d = 1;
	while (d <= val)
	    d *= 10;

	while (d > 10)
	{
	    d /= 10;
	    int x = val / d;
	    buffer.push_back('0'+x);
	    val -= x * d;
	}
	buffer.push_back('0' + val);
	assertPrint("Converting 64-bit integers to string 1/6:", buffer.c_str()); 
	//CHECK: Converting 64-bit integers to string 1/6: 100
	char charBuffer[32];
	d = 100;
	sprintf(charBuffer,"%llu",d);
	assertPrint("Converting 64-bit integers to string 2/6:", buffer.c_str()); //CHECK: Converting 64-bit integers to string 2/6: 100
	d = 0xffffffffLL;
	sprintf(charBuffer,"%llu",d);
	assertPrint("Converting 64-bit integers to string 3/6:", charBuffer); //CHECK: Converting 64-bit integers to string 3/6: 4294967295
	sprintf(charBuffer,"%lli",d);
	assertPrint("Converting 64-bit integers to string 4/6:", charBuffer); //CHECK: Converting 64-bit integers to string 4/6: 4294967295
	d = 0xffffffffffffffffLL;
	sprintf(charBuffer,"%llu",d);
	assertPrint("Converting 64-bit integers to string 5/6:", charBuffer); //CHECK: Converting 64-bit integers to string 5/6: 18446744073709551615
	sprintf(charBuffer,"%lli",d);
	assertPrint("Converting 64-bit integers to string 6/6:", charBuffer); //CHECK: Converting 64-bit integers to string 6/6: -1
	sscanf("100", "%llu", &d);
	assertPrint("Converting string to 64-bit integers 1/5:", d); //CHECK: Converting string to 64-bit integers 1/5: 100
	sscanf("4294967295", "%llu", &d);
	assertPrint<uint64_t>("Converting string to 64-bit integers 2/5:", d); //CHECK: Converting string to 64-bit integers 2/5: 4294967295
	sscanf("4294967295", "%lli", &d);
	assertPrint<uint64_t>("Converting string to 64-bit integers 3/5:", d); //CHECK: Converting string to 64-bit integers 3/5: 4294967295
	sscanf("18446744073709551615", "%llu", &d);
	assertPrint<uint64_t>("Converting string to 64-bit integers 4/5:", Hex(d)); //CHECK: Converting string to 64-bit integers 4/5: 0xffffffffffffffff 
	sscanf("-1", "%lli", &d);
	assertPrint<uint64_t>("Converting string to 64-bit integers 5/5:", Hex(d)); //CHECK: Converting string to 64-bit integers 5/5: 0xffffffffffffffff
}
