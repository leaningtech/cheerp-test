//===---------------------------------------------------------------------===//
//      Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s

#include <tests.h>

struct A
{
	int a;
};

struct B
{
	A* a;
};

void __attribute__((noinline)) func(A** a)
{
	*a = NULL;
}

void webMain()
{
	B b { new A() };
	func(&b.a);
	A* volatile a2 = b.a;
	assertEqual<A *>((A *)a2, (A *)nullptr, "Pointers to structs in structs: SUCCESS");
	//CHECK: Pointers to structs in structs: SUCCESS
}