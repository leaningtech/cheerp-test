//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: mkdir -p %t

// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1| %FileCheck %s
// RUN: preexec_only run_if_js compile_mode_js -o %t/j %s 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 | %FileCheck %s


#include <tests.h>

class A
{
public:
	char a;
};

class B: public A
{
public:
	int b;
	B():b(0xdeadbeaf)
	{
		a=42;
	}
};

class C
{
public:
	short a;
	unsigned int b : 1;
	C():a(3),b(1)
	{
	}
};

void webMain()
{
	B b;

	assertPrint("Alignment at the end of class 1/3:", b.a);
	// CHECK: Alignment at the end of class 1/3: *
	// assertEqual<int>(b.b, 0xdeadbeaf, "Alignment at the end of class [2/3]");
	assertPrint("Alignment at the end of class 2/3:", Hex((unsigned int)b.b));
	// CHECK: Alignment at the end of class 2/3: 0xdeadbeaf
	volatile C c;
	// assertEqual((short)c.a, (short)3, "Alignment at the end of class [3/3]");
	assertPrint("Alignment at the end of class 3/3:", (short)c.a);
	// CHECK: Alignment at the end of class 3/3: 3
}
