//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: js, regular

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 %then_run_js | %FileCheck %s

#include <cheerp/client.h>
#include <tests.h>

static int ConsCount = 0;
static int DestCount = 0;
static int CopyCount = 0;
static int MoveCount = 0;
struct A {
	int i;
	A(int i): i(i) {
		ConsCount++;
	}
	~A(){
		DestCount++;
	}
	A(const A& a): i(a.i) {
		CopyCount++;
	}
	A(A&& a): i(a.i) {
		MoveCount++;
	}
};

int foo(int a)
{
	return a+unitBlackBox(2*a);
}
void webMain()
{
	A a(1);
	auto l = [a{std::move(a)}](int b)
	{
		return a.i+b;
	};

	{
		auto cl = cheerp::make_closure(std::move(l));
		auto cl1 = std::move(cl);
		assertEqual(cl1(unitBlackBox(2)), 3, "Create Closure from lambda with captures 1/1:");
	}
	assertPrint("Lambda object lifecycle 1/4:", ConsCount);
	assertPrint("Lambda object lifecycle 2/4:", DestCount);
	assertPrint("Lambda object lifecycle 3/4:", CopyCount);
	assertPrint("Lambda object lifecycle 4/4:", MoveCount);

	// CHECK: Create Closure from lambda with captures 1/1: 3
	// CHECK: Lambda object lifecycle 1/4: 1
	// CHECK: Lambda object lifecycle 2/4: 1
	// CHECK: Lambda object lifecycle 3/4: 0
	// CHECK: Lambda object lifecycle 4/4: 2

	auto cl2 = cheerp::make_closure([](int a)
	{
		return a+unitBlackBox(a);
	});
	assertPrint("Create Closure from lambda without captures 1/1:", cl2(unitBlackBox(2)));
	// assertEqual(cl2(unitBlackBox(2)), 4, "Create Closure from lambda without captures 1/1");

	cl2 = cheerp::Closure<int(int)>(&foo);
	assertPrint("Create Closure from function pointer 1/1:", cl2(unitBlackBox(2)));
	// assertEqual(cl2(unitBlackBox(2)), 6, "Create Closure from function pointer 1/1");

	// CHECK: Create Closure from lambda without captures 1/1: 4
	// CHECK: Create Closure from function pointer 1/1: 6

}
