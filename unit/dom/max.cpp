
//===---------------------------------------------------------------------===//
//	Copyright 2019-2021 Leaning Technlogies
//===----------------------------------------------------------------------===//


// REQUIRES: js

// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// CHECK: Variadic Math.max 1/3 : 10
// CHECK: Variadic Math.max 2/3 : SUCCESS
// CHECK: Variadic Math.max 3/3 : SUCCESS


#include <tests.h>
#include <cheerp/client.h>

void webMain()
{
	// Test variadic client class
	double m1 = client::Math.max(1.0f, 10u);
	client::console.log("Variadic Math.max 1/3 : ", m1);
	double m2 = client::Math.max(20u, 30u);
	assertEqual(m2, 30.0, "Variadic Math.max 2/3");
	double m3 = client::Math.max(0.1, 0.2);
	assertEqual(m3, 0.2, "Variadic Math.max 3/3");
}

