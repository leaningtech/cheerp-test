// XFAIL: *
// NOTE: This test requires .testing.js harness infrastructure not yet implemented in lit



// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp -cheerp-make-module=commonjs %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp -cheerp-make-module=es6 %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs -cheerp-make-module=commonjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs -cheerp-make-module=es6 %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-make-module=commonjs %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-make-module=es6 %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: Check counter 1/3 : SUCCESS
// CHECK: Check counter 2/3 : SUCCESS
// CHECK: Call actual catch : SUCCESS
// CHECK: Check counter 3/3 : SUCCESS

#include <tests.h>

namespace [[cheerp::genericjs]] client
{
	class [[cheerp::client_layout]] ToBeDeleted
	{
	public:
		ToBeDeleted(int a);
		[[cheerp::interface_name(("delete"))]] void _delete();
		[[cheerp::interface_name(("catch"))]] int _catch();
	};
	int counterAlive();
	template <class T>
	[[cheerp::interface_name(("assert"))]]
	void _assert(T& someObject);
}

[[cheerp::genericjs]]
int main()
{
	assertEqual(client::counterAlive(), 0, "Check counter 1/3");

	client::ToBeDeleted* TBD = new client::ToBeDeleted(123);
	client::_assert<client::ToBeDeleted>(*TBD);
	assertEqual(client::counterAlive(), 1, "Check counter 2/3");
	assertEqual(TBD->_catch(), 123, "Call actual catch");
	
	TBD -> _delete();

	assertEqual(client::counterAlive(), 0, "Check counter 3/3");
}
