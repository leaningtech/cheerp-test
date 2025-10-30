//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: asmjs

// RUN: mkdir -p %t
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1 | %FileCheck %s


#include <tests.h>
#include <vector>

void process(std::vector<int>& v) {
	int k = 0;
	for(int i = 0; i < v.size(); i++) {
		v[i] += k;
		k = v[i];
	}
}

template<typename T>
void push_back_wrapper(std::vector<T>& v, T i)
{
	v.push_back(i);
}
[[cheerp::genericjs]]
int generic() {
	std::vector<int> v;
	for(int i = 0; i < 5; i++) {
		push_back_wrapper(v, i);
	}
	process(v);
	int ret = v[4];
	return ret;
}
void webMain()
{
	assertPrint("ffi pointer interoperation:", generic());
	// CHECK: ffi pointer interoperation: 10
}
