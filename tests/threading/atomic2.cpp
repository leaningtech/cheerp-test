//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// REQUIRES: wasm, regular


// testing vanilla
// RUN: %compile -pthread -o %t-vanilla.js %s 2>&1
// RUN: python3 %helpers/create_driver.py %t-vanilla.js %S/atomic_common.testing.js %t-vanilla_driver.js --module=vanilla --pthread
// RUN: %node %t-vanilla_driver.js 2>&1 | %FileCheck %s

// testing es6
// RUN: %compile -pthread -cheerp-make-module=es6 -o %t-es6.mjs %s 2>&1
// RUN: python3 %helpers/create_driver.py %t-es6.mjs %S/atomic_common.testing.js %t-es6_driver.mjs --module=es6 --pthread
// RUN: %node %t-es6_driver.mjs 2>&1 | %FileCheck %s

// testing closure
// RUN: %compile -pthread -cheerp-make-module=closure -o %t-closure.js %s 2>&1
// RUN: python3 %helpers/create_driver.py %t-closure.js %S/atomic_common.testing.js %t-closure_driver.js --module=closure --pthread
// RUN: %node %t-closure_driver.js 2>&1 | %FileCheck %s

// testing genericjs
// RUN: %compile -pthread -cheerp-make-module=commonjs -o %t-commonjs.js %s 2>&1
// RUN: python3 %helpers/create_driver.py %t-commonjs.js %S/atomic_common.testing.js %t-commonjs_driver.js --module=commonjs --pthread
// RUN: %node %t-commonjs_driver.js 2>&1 | %FileCheck %s

#include <tests.h>
#include <atomic>

struct Node
{
	int value;
	Node* next;
};
std::atomic<Node*> listHead(nullptr);

void appendToList(int newValue)
{
	Node* oldHead = listHead;
	Node* newNode = new Node {newValue, oldHead};

	while (!listHead.compare_exchange_strong(oldHead, newNode))
		newNode->next = oldHead;
}

int main()
{
	for (int i = 0; i < 10; i++)
		appendToList(i);

	Node* it = listHead;
	it = it->next->next->next;
	assertPrint("Compare exchange list:", it->value);
	// CHECK: Compare exchange list: 6
}
