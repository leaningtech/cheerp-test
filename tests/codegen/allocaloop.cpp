//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


#include <tests.h>

struct node
{
	node* next;
	int value;
};

void webMain()
{
	// Test that a constant-size alloca executed several times returns fresh memory each time
	volatile int count = 4;
	node* last = nullptr;
	for (int i = 0; i < count; i++)
	{
		node* n = static_cast<node*>(alloca(sizeof(node)));
		n->next = last;
		n->value = i;
		last = n;
	}
	int sum = 0;
	int length = 0;
	for (node* n = last; n && length < count + 1; n = n->next)
	{
		sum += n->value;
		length++;
	}
	assertPrint("Alloca in a loop 1/1:", length * 100 + sum);
	// CHECK: Alloca in a loop 1/1: 406
}
