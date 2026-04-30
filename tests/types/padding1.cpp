//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// REQUIES: regular

// REQUIRES: regular
// RUN: %compile -cheerp-fix-wrong-func-casts -o %t.js %s 2>%t.log && %run | %FileCheck %s


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

