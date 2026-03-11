//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// REQUIRES: linear-memory

// RUN: mkdir -p %t

// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1| %FileCheck %s



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
	assertPrint("compare exchange list:", it->value);
	// CHECK: compare exchange list: 6
}
