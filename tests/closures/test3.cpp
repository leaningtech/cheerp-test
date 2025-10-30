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
int eventEmitter(client::EventListener* e)
{
	int res = reinterpret_cast<int(*)(int)>(e)(unitBlackBox(1));
	return res;
}
void webMain()
{
	A a(1);
	auto l = [a{std::move(a)}](int b)
	{
		return a.i+b;
	};

	client::EventListener* el = nullptr;
	{
		auto cb = cheerp::make_closure(std::move(l));
		el = cb;
		assertPrint("Calling closures through EventListener:", eventEmitter(el));
		// CHECK: Calling closures through EventListener: 2
	}
	assertPrint("Escaping closure lifecycle 1/2:", DestCount);
	// CHECK: Escaping closure lifecycle 1/2: 0
	cheerp::freeCallback(el);
	assertPrint("Escaping closure lifecycle 2/2:", DestCount);
	// CHECK: Escaping closure lifecycle 2/2: 1

}





