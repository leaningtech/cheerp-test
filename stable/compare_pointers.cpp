// Test pointer comparison with jsexport across all targets
// RUN: mkdir -p %t
// Preexecution mode (PREEXECUTE_MODE=true):
// RUN: preexec_only run_if_js %valgrind compile_mode_js -o %t/j_pre %s 2>&1 | %FileCheck %s 
// RUN: preexec_only run_if_asmjs %valgrind compile_mode_asmjs -o %t/a_pre %s 2>&1 | %FileCheck %s
// 
// Regular mode (PREEXECUTE_MODE=false):
// RUN: compile_for_wasm -O1 -o %t/w %s
// RUN: compile_for_js -O1 -o %t/j %s
// RUN: compile_for_asmjs -O1 -o %t/a %s
//
// RUN: run_if_wasm test -f %t/w && test -f %t/w.wasm
// RUN: run_if_js test -f %t/j
// RUN: run_if_asmjs test -f %t/a
//
// RUN: run_if_wasm %node %t/w 2>&1
// RUN: run_if_js %node %t/j 2>&1
// RUN: run_if_asmjs %node %t/a 2>&1

//CHECK: Compare jsexported pointers 1/2 SUCCESS
//CHECK: Compare jsexported pointers 2/2 SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2022 Leaning Technologies
//===----------------------------------------------------------------------===//

#include <tests.h>

#include <iostream>
#include <cheerp/client.h>

#include <cheerp/client.h>

struct [[cheerp::jsexport]] Bar;
struct [[cheerp::jsexport]] Foo
{
	int i{1};
	static Foo* create()
	{
		return new Foo();
	}
	int getI()
	{
		return i;
	}
	[[cheerp::genericjs]]
	bool cmp(Bar* b);
};
struct [[cheerp::jsexport]] Bar
{
	int i{1};
	Foo* f;
	static Bar* create()
	{
		auto* b = new Bar();
		auto* ff = new Foo[10];
		b->f = ff;
		return b;
	}
	Foo* getFoo(int i)
	{
		return &f[i];
	}
	bool cmp(Foo* f)
	{
		return &this->f[2] == &f[1];
	}
};
namespace [[cheerp::genericjs]] client
{
	struct Callback: public Object
	{
		Foo* operator()(Foo* f);
	};
	extern Callback jsFunc;
}

[[cheerp::genericjs]]
bool Foo::cmp(Bar* b)
{
	Foo* f1 = client::jsFunc(&this[1]);
	Foo* f2 = client::jsFunc(&b->f[2]);
	return f1 == f2;
}

int main()
{
	return 0;
}
