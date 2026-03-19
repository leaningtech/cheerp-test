//===---------------------------------------------------------------------===//
//	Copyright 2022 Leaning Technologies
//===----------------------------------------------------------------------===//

// Test pointer comparison with jsexport across all targets
//
// UNSUPPORTED: asan
// NOTE: ASAN is unsupported for JS target, see lit.cfg changes for details
// 
// RUN: mkdir -p %t
// 
// Test vanilla module
// RUN: regular_only run_if_js compile_mode_js -o %t/vanilla.js %s 2>&1
// RUN: regular_only run_if_js python3 %S/../helpers/create_driver.py %t/vanilla.js %S/compare_pointers.testing.js %t/vanilla_driver.js --module=vanilla
// RUN: regular_only run_if_js %node %t/vanilla_driver.js 2>&1 | %FileCheck %s --check-prefix=CHECK

// Test ES6 module
// RUN: regular_only run_if_js compile_mode_js -cheerp-make-module=es6 -o %t/es6.mjs %s 2>&1
// RUN: regular_only run_if_js python3 %S/../helpers/create_driver.py %t/es6.mjs %S/compare_pointers.testing.js %t/es6_driver.mjs --module=es6
// RUN: regular_only run_if_js %node %t/es6_driver.mjs 2>&1 | %FileCheck %s --check-prefix=CHECK

// Test CommonJS module
// RUN: regular_only run_if_js compile_mode_js -cheerp-make-module=commonjs -o %t/commonjs.js %s 2>&1
// RUN: regular_only run_if_js python3 %S/../helpers/create_driver.py %t/commonjs.js %S/compare_pointers.testing.js %t/commonjs_driver.js --module=commonjs
// RUN: regular_only run_if_js %node %t/commonjs_driver.js 2>&1 | %FileCheck %s --check-prefix=CHECK

// Test Closure module
// RUN: regular_only run_if_js compile_mode_js -cheerp-make-module=closure -o %t/closure.js %s 2>&1
// RUN: regular_only run_if_js python3 %S/../helpers/create_driver.py %t/closure.js %S/compare_pointers.testing.js %t/closure_driver.js --module=closure
// RUN: regular_only run_if_js %node %t/closure_driver.js 2>&1 | %FileCheck %s --check-prefix=CHECK
//
// Also test wasm and asmjs with vanilla driver:
// RUN: regular_only run_if_wasm %valgrind compile_mode_wasm -o %t/w %s 2>&1
// RUN: regular_only run_if_wasm python3 %S/../helpers/create_driver.py %t/w %S/compare_pointers.testing.js %t/w_driver.js --module=vanilla
// RUN: regular_only run_if_wasm %node %t/w_driver.js 2>&1 | %FileCheck %s
//
// RUN: regular_only run_if_asmjs %valgrind compile_mode_asmjs -o %t/a %s 2>&1
// RUN: regular_only run_if_asmjs python3 %S/../helpers/create_driver.py %t/a %S/compare_pointers.testing.js %t/a_driver.js --module=vanilla
// RUN: regular_only run_if_asmjs %node %t/a_driver.js 2>&1 | %FileCheck %s 


//CHECK: Compare jsexported pointers 1/2 SUCCESS
//CHECK: Compare jsexported pointers 2/2 SUCCESS

#include <tests.h>
#include <iostream>
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
