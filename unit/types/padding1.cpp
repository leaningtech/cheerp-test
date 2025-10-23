// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %node %t.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %node %t_asmjs.js 2>&1 | %FileCheck %s
// RUN: %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js
// RUN: %node %t_wasm.js 2>&1 | %FileCheck %s
// CHECK: Padding support : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2021 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>
#include <vector>


class basic_json
{
public:
        basic_json()
        {
        }

public:
        /// the type of the current element
        char m_type = 0;
        long long m_value = 0;
	char x = 8;
};

template <unsigned int N>
void testSize()
{
	std::vector<basic_json> v(N);
	assertEqual(v.size(), N, "Padding support");
}

int main()
{
	testSize<1>();
	testSize<2>();
	testSize<3>();
	testSize<57>();
	return 0;
}

