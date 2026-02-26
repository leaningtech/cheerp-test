//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: js, regular

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j | %FileCheck %s

#include <tests.h>
#include <cheerp/client.h>

void webMain()
{
	// Test variadic client class
	double m1 = client::Math.max(1.0f, 10u);
	assertPrint("Math.max(1.0f, 10u) =", m1);
	// CHECK: Math.max(1.0f, 10u) = 10.0 
	double m2 = client::Math.max(20u, 30u);
	assertPrint("Math.max(20u, 30u) =", m2);
	// CHECK: Math.max(20u, 30u) = 30.0
	double m3 = client::Math.max(0.1, 0.2);
	assertPrint("Math.max(0.1, 0.2) =", m3);
	// CHECK: Math.max(0.1, 0.2) = 0.2
	client::console.log("Variadic", "console.log", ":", "SUCCESS");
	// CHECK: Variadic console.log : SUCCESS
}
