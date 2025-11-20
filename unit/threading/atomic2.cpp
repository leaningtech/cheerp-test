


// REQUIRES: wasm

// This test uses threading-wrapper to provide Worker polyfills for pthread support
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -pthread -cheerp-make-module=es6 %s -o %t.mjs
// RUN: %S/../../test-wrappers/threading-wrapper.mjs %t.mjs 2>&1 | %FileCheck %s

// CHECK: Compare exchange list : SUCCESS

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
	assertEqual(it->value, 6, "Compare exchange list");
}
