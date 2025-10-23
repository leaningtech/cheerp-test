// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: Calls through lambda : SUCCESS

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
		assertEqual(A, n, "Calls through lambda");
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
