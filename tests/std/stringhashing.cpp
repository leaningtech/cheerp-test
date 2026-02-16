//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

// REQUIRES: js

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck %s

#include <tests.h>
#include <string>
#include <unordered_set>

void testUnorderedSetOfString()
{
	std::unordered_set<std::string> a;
	assertPrint("std::set<std::string> 1/4:", a.size());
	// CHECK: std::set<std::string> 1/4: 0
	a.insert("1");
	assertPrint("std::set<std::string> 2/4:", a.size());
	// CHECK: std::set<std::string> 2/4:
	a.insert("10");
	a.insert("99");
	a.insert("100");
	a.insert("100");
	a.insert("-2");
	assertPrint("std::set<std::string> 3/4:", a.size());
	// CHECK: std::set<std::string> 3/4: 5
	const char* expected[] = {"-2", "99", "100", "10", "1"};
	std::unordered_set<std::string>::iterator it = a.begin();
	bool correctOrder = true;
	for(int i=0;i<a.size();i++)
	{
		if(*it != expected[i])
			correctOrder = false;
		++it;
	}
	assertPrint("std::set<std::string> ordering 4/4:", correctOrder);
	// CHECK: std::set<std::string> ordering 4/4: 1
}

void testUnorderedSetOfWString()
{
	std::unordered_set<std::wstring> a;
	assertPrint("std::set<std::wstring> 1/4:", a.size());
	// CHECK: std::set<std::wstring> 1/4: 0
	a.insert(L"1");
	assertPrint("std::set<std::wstring> 2/4:", a.size());
	// CHECK: std::set<std::wstring> 2/4: 1
	a.insert(L"10");
	a.insert(L"99");
	a.insert(L"100");
	a.insert(L"100");
	a.insert(L"-2");
	assertPrint("std::set<std::wstring> 3/4:", a.size());
	// CHECK: std::set<std::wstring> 3/4: 5
	const wchar_t* expected[] = {L"100", L"-2", L"99", L"10", L"1"};
	std::unordered_set<std::wstring>::iterator it = a.begin();
	bool correctOrder = true;
	for(int i=0;i<a.size();i++)
	{
		if(*it != expected[i])
			correctOrder = false;
		++it;
	}
	assertPrint("std::set<std::wstring> 4/4:", correctOrder);
	//CHECK: std::set<std::wstring> 4/4: 1
}

void webMain()
{
	testUnorderedSetOfString();
	testUnorderedSetOfWString();
}
