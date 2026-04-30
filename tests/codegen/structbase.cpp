//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s

#include <tests.h>

struct a {
	short b;
	unsigned int c;
	int d;
};
struct {
	struct a b;
} e = {9, 8};
int *f = &e.b.d;
void g() { *f = 234; }

int main() {
	g();
	if (e.b.c == 8u)
		assertPrint("e.b.c:", 8);
	// CHECK: e.b.c: 8
}
