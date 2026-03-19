//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: mkdir -p %t

// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 && node %t/a 2>&1| %FileCheck %s
// RUN: preexec_only run_if_js compile_mode_js -o %t/j %s 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1 | %FileCheck %s

#include <tests.h>
#include <algorithm>
#include <vector>
#include <string>

struct S
{
    int32_t i;
    std::string s;
    bool operator<(const S& r) const
    {
        return this->i < r.i;
    }
};

std::vector<S> v;

void webMain()
{
	std::sort(v.begin(),v.end());
    assertPrint("std::sort on complex structures:", v.size());
    // CHECK: std::sort on complex structures: 0
}
