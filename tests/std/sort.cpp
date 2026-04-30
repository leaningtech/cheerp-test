//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===---------------------------------------------------------------------===//

// RUN: %compile -o %t.js %s 2>%t.log && %run | %FileCheck %s


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
