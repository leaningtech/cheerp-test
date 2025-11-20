// Test pimpl pattern with jsexport across all targets
// RUN: mkdir -p %t
// Preexecution mode (PREEXECUTE_MODE=true):
// RUN: preexec_only run_if_js %valgrind compile_mode_js -o %t/j_pre %s 2>&1 | %FileCheck %s --check-prefix=PREEXEC_OK
// RUN: preexec_only run_if_asmjs %valgrind compile_mode_asmjs -o %t/a_pre %s 2>&1 | %FileCheck %s --check-prefix=PREEXEC_OK
//
// Regular mode (PREEXECUTE_MODE=false):
// RUN: regular_only run_if_wasm %valgrind compile_mode_wasm -o %t/w %s 2>&1 && (%node %t/w 2>&1) | %FileCheck %s --check-prefix=REG_OK
// RUN: regular_only run_if_js %valgrind compile_mode_js -o %t/j %s 2>&1 && (%node %t/j 2>&1) | %FileCheck %s --check-prefix=REG_OK
// RUN: regular_only run_if_asmjs %valgrind compile_mode_asmjs -o %t/a %s 2>&1 && (%node %t/a 2>&1) | %FileCheck %s --check-prefix=REG_OK
//
// PREEXEC_OK-NOT: error
// REG_OK: JSExport non-trivial destructor 1/1
// REG_OK: JSExport deleter check 1/1



//===---------------------------------------------------------------------===//
//	Copyright 2021 Leaning Technlogies
//===----------------------------------------------------------------------===//

// #include <tests.h>
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
