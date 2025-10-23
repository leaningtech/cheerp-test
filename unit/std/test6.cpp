// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: List push_back 1/2 : SUCCESS
// CHECK: List push_back 2/2 : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2014 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>
#include <list>

using namespace std;

void webMain()
{
	struct A
	{
		list<int> v;
		int a,b;
	} obj;

	list<A> l;

	l.push_back(move(obj));
	assertEqual(l.size(), 1u, "List push_back 1/2");
	l.push_back(obj);
	assertEqual(l.size(), 2u, "List push_back 2/2");
}
