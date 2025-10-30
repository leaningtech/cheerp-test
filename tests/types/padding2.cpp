//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// REQUIES: regular

// RUN: mkdir -p %t

// RUN: regular_only run_if_js compile_mode_js -cheerp-fix-wrong-func-casts -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -cheerp-fix-wrong-func-casts -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -cheerp-fix-wrong-func-casts -o %t/a %s 2>&1 && node %t/a 2>&1| %FileCheck %s

#include <tests.h>
#include <vector>


struct A
{
public:
        A() {}
        char m_type = 0;
        long long m_value = 0;
};

struct B
{
public:
	B() {}
	char some_char = 2;
	A a;
	char some_other_char = 23;
	A z;
	long long some_64 = 123;
};

template <unsigned int N>
void testSize()
{
	std::vector<B> v(N);
	assertPrint("Padding support:", v.size());
}

int main()
{
	testSize<1>();
	// CHECK: Padding support: 1
	testSize<2>();
	// CHECK: Padding support: 2
	testSize<3>();
	// CHECK: Padding support: 3
	testSize<57>();
	// CHECK: Padding support: 57
	return 0;
}

