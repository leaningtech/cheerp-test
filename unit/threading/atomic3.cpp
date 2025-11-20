


// REQUIRES: wasm

// This test uses threading-wrapper to provide Worker polyfills for pthread support
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -pthread -cheerp-make-module=es6 %s -o %t.mjs
// RUN: %S/../../test-wrappers/threading-wrapper.mjs %t.mjs 2>&1 | %FileCheck %s

// CHECK: Simple compare exchange : SUCCESS

#include <tests.h>
#include <atomic>

std::atomic<int> value(0);

void addToNumber()
{
	int num = value.load();
	int newNum = num + 1;
	while (!value.compare_exchange_weak(num, newNum))
		newNum = num + 1;
}

int main()
{
	for (int i = 0; i < 100; i++)
		addToNumber();
	assertEqual(value.load(), 100, "Simple compare exchange");
}
