//===---------------------------------------------------------------------===//
//	Copyright 2021 Leaning Technlogies
//===----------------------------------------------------------------------===//

// Test pimpl pattern with jsexport across all targets and module types
// RUN: mkdir -p %t
// 
// Regular mode (PREEXECUTE_MODE=false) - Test all 4 module types (vanilla, ES6, CommonJS, closure):
// RUN: regular_only run_if_js %S/test_all_modules.sh %s %S/cheerp_pimpl_mod.testing.js %t compile_mode_js 2>&1 | %FileCheck %s 
//
// Also test wasm and asmjs with vanilla driver:
// RUN: regular_only run_if_wasm %valgrind compile_mode_wasm -o %t/w %s 2>&1
// RUN: regular_only run_if_wasm python3 %S/create_driver.py %t/w %S/cheerp_pimpl_mod.testing.js %t/w_driver.js --module=vanilla
// RUN: regular_only run_if_wasm %node %t/w_driver.js 2>&1 | %FileCheck %s
//
// RUN: regular_only run_if_asmjs %valgrind compile_mode_asmjs -o %t/a %s 2>&1
// RUN: regular_only run_if_asmjs python3 %S/create_driver.py %t/a %S/cheerp_pimpl_mod.testing.js %t/a_driver.js --module=vanilla
// RUN: regular_only run_if_asmjs %node %t/a_driver.js 2>&1 | %FileCheck %s 
//
// CHECK: JSExport non-trivial destructor 1/1
// CHECK: JSExport deleter check 1/1
// CHECK-NOT: FAILURE


#include "./jsexport/cheerp_pimpl.h"
#include <cheerp/client.h>
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
                if(totalInserted() >= uniqueInserted())
                        cheerp::console_log("JSExport non-trivial destructor 1/1");
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

int main()
{
	invokeCpp();
	return 0;
}
