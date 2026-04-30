//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: regular
// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


// CHECK: Calling functor through EventListener 1/1: 2
// CHECK: Calling std::function through EventListener 1/1: 2
// CHECK: Escaping std::function lifecycle 1/2: 1
// CHECK: Escaping std::function lifecycle 2/2: 2

#include <cheerp/client.h>
#include <functional>
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
	int operator()(int b)
	{
		return i+b;
	}
};

[[cheerp::genericjs]]
int eventEmitter(client::EventListener* e)
{
	int res = reinterpret_cast<int(*)(int)>(e)(unitBlackBox(1));
	return res;
}
[[cheerp::genericjs]]
void webMain()
{
	A a(1);
	client::EventListener* el = nullptr;
	{
		el = cheerp::Callback(a);
		assertPrint("Calling functor through EventListener 1/1:", eventEmitter(el));
		el = cheerp::Callback(std::function<int(int)>(std::move(a)));
		assertPrint("Calling std::function through EventListener 1/1:", eventEmitter(el));
	}
	assertPrint("Escaping std::function lifecycle 1/2:", DestCount);
	cheerp::freeCallback(el);
	assertPrint("Escaping std::function lifecycle 2/2:", DestCount);
}
