//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technlogies
//===----------------------------------------------------------------------===//

// RUN: mkdir -p %t
// REQUIRES: regular
//
// Test vanilla module
// RUN: regular_only run_if_js compile_mode_js -o %t/vanilla.js %s 2>&1
// RUN: regular_only run_if_js %node %t/vanilla.js 2>&1 | %FileCheck %s
//
// Test ES6 module
// RUN: regular_only run_if_js compile_mode_js -cheerp-make-module=es6 -o %t/es6.mjs %s 2>&1
// RUN: regular_only run_if_js python3 %helpers/run_cheerp_module.py --module=es6 %t/es6.mjs 2>&1 | %FileCheck %s
//
// Test CommonJS module
// RUN: regular_only run_if_js compile_mode_js -cheerp-make-module=commonjs -o %t/commonjs.js %s 2>&1
// RUN: regular_only run_if_js python3 %helpers/run_cheerp_module.py --module=commonjs %t/commonjs.js 2>&1 | %FileCheck %s
//
// Test Closure module
// RUN: regular_only run_if_js compile_mode_js -cheerp-make-module=closure -o %t/closure.js %s 2>&1
// RUN: regular_only run_if_js %node %t/closure.js 2>&1 | %FileCheck %s
//
// Also test wasm and asmjs with vanilla driver:
// RUN: regular_only run_if_wasm compile_mode_wasm -o %t/w %s 2>&1
// RUN: regular_only run_if_wasm %node %t/w 2>&1 | %FileCheck %s
//
// RUN: regular_only run_if_asmjs compile_mode_asmjs -o %t/a %s 2>&1
// RUN: regular_only run_if_asmjs %node %t/a 2>&1 | %FileCheck %s
//
// CHECK: JSExport non-trivial destructor: 1
// CHECK: JSExport non-trivial destructor: 1
// CHECK: JSExport deleter check: 0

// CHECK-NOT: FAILURE

#include <tests.h>
#include <jsexport/cheerp_pimpl.h>
#include <vector>
#include <set>
#include <iostream>

//Initialize the counter from cheerp_pimpl.h
int CounterAlive::counter = 0;

//Arbitrary class with no limitation of sort (non-trivial destructor here)
class ArbitraryClass
{
public:
        ArbitraryClass(int N)
        {
		for (int i=0; i<N; i++)
		{
			insert(i);
			insert(i);
		}
        }
        void insert(int N)
        {
                //Insert N both in the set and the vector
                V.push_back(N);
                S.insert(N);
        }
        int totalInserted() const
        {
                return V.size();
        }
        int uniqueInserted() const
        {
                return S.size();
        }
        ~ArbitraryClass()
        {
                assertPrint("JSExport non-trivial destructor:", totalInserted() >= uniqueInserted());
		// assertEqual(totalInserted() >= uniqueInserted(), true, "JSExport non-trivial destructor 1/1");
        }

private:
        std::vector<int> V;
        std::set<int> S;
};


//JSExported wrapper, all relevant function have to be redeclared and have them call the relevant function on the implementation
class [[cheerp::jsexport]][[cheerp::genericjs]] JSExportedWrapperWithDeleter
{
        cheerp_pimpl::pimpl_with_deleter<ArbitraryClass> m;
public:
        JSExportedWrapperWithDeleter(int N) : m(N)
					//Initialize ArbitraryClass with the value N
        {
        }
        void insert(int N)
        {
                getInner().insert(N);
        }
	int totalInserted() const
	{
		return getInner().totalInserted();
	}
//      ~JSExportedWrapperWithDeleter()  //We can't have the destructor... for now we are forced to declare a deleter
        void deleter()
        {
                m.deleter();
        }
private:
	ArbitraryClass& getInner()
	{
		return *m;
	}
	const ArbitraryClass& getInner() const
	{
		return *m;
	}
};


//Use of ArbitraryClass from C++ code
void invokeCpp()
{
        ArbitraryClass W(21);
        W.insert(45);
        W.insert(2134);
        //W destructor called automatically...
}

//Use of JSExportedWrapperWithDeleter of ArbitraryClass from JavaScript code
[[cheerp::genericjs]]
void invokeJavaScript()
{
        //__asm__ is a way of directing calling JavaScript arbitrary code, but in principle similar code should be on the user side
        __asm__("\
                var instance = new JSExportedWrapperWithDeleter(21);\
                instance.insert(45);\
                instance.insert(2134);\
		var X = instance.totalInserted(); \
                instance.deleter();\
                ");
                //In JS we MUST call the deleter
}

int main()
{
        invokeCpp();
        invokeJavaScript();
        assertPrint("JSExport deleter check:", CounterAlive::numberAlive());
        // You might be willing to create some objects and leave them alive, but be conscious with memory leaks
        return 0;
}
