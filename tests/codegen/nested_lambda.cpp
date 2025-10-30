//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

//REQUIRES: regular

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1 | %FileCheck %s



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