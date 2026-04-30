//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: regular
// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s




#include <tests.h>

namespace [[cheerp::genericjs]] client
{
	class ClientClass : public Object
	{
	public:
		ClientClass(const int N);
		void doTest(const int A);
	};
	void someFunc();
	void someFuncNested();
};

class [[cheerp::genericjs]][[cheerp::jsexport]] ClientClass
{
public:
	ClientClass(const int N) : n(N) {}
	void doTest(const int A)
	{
		if (A == n)
			assertPrint("Calls through lambda:", A);
		// assertPrint("Calls through lambda", n);
		// assertEqual(A, n, "Calls through lambda");
	}
private:
	const int n;
};

void [[cheerp::genericjs]][[cheerp::jsexport]] someFunc()
{
	auto lambda = [](client::ClientClass* instance) -> void
	{
		instance->doTest(123);
	};

	client::ClientClass* X = new client::ClientClass(123);
	lambda(X);
}

void [[cheerp::genericjs]][[cheerp::jsexport]] someFuncNested()
{
	auto lambda_outer = [](client::ClientClass* instance) -> void
	{
		auto lambda = [](client::ClientClass* a) -> void
		{
			a->doTest(42);
		};

		lambda(instance);
		lambda(instance);
	};

	client::ClientClass* X = new client::ClientClass(42);
	lambda_outer(X);
}

int main()
{
	someFunc();
	someFuncNested();
}

// CHECK: Calls through lambda: 123
// CHECK: Calls through lambda: 42
// CHECK: Calls through lambda: 42