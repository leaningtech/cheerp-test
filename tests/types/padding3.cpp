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
	char XXX = 23;
	void addStuff(int X)
	{
		m_type += X;
		m_value *= X;
	}
	int hash() const
	{
		return m_value * m_type;
	}
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
	void addStuff(int X)
	{
		some_64 += X;
		some_other_char ^= X;
		a.addStuff(some_64++);
		z.addStuff(some_other_char++);
		some_64 += some_other_char;
	}
	int hash()
	{
		return a.hash() + z.hash() * some_other_char + some_64;
	}
};

template <unsigned int N>
int testSize()
{
	std::vector<B> v(N);
	for (int i=0; i<N*3; i++)
	{
		v[i%N].addStuff(i);
	}
	int hash = 123;
	for (int i=0; i<N; i++)
	{
		hash += v[i].hash();
	}

	assertPrint("Padding support:", v.size());


	return hash;
}

int main()
{
	int sum = 0;
	sum += testSize<1>();
	sum += testSize<2>();
	sum += testSize<3>();
	sum += testSize<57>();
	// CHECK: Padding support: 1
	// CHECK: Padding support: 2
	// CHECK: Padding support: 3
	// CHECK: Padding support: 57
	assertPrint("Access to member test:", sum);
	// CHECK: Access to member test: 29198
	return 0;
}

