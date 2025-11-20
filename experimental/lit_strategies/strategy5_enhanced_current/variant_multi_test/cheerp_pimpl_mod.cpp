// Test pimpl pattern with jsexport across all targets
// RUN: mkdir -p %t
// RUN: %compile_for{wasm} -O1 -o %t/w %s
// RUN: %compile_for{js} -O1 -o %t/j %s
// RUN: %compile_for{asmjs} -O1 -o %t/a %s
//
// RUN: %run_if{wasm} test -f %t/w && test -f %t/w.wasm
// RUN: %run_if{js} test -f %t/j
// RUN: %run_if{asmjs} test -f %t/a
//
// RUN: %run_if{wasm} %node %t/w 2>&1 | %FileCheck %s --check-prefix=CHECK
// RUN: %run_if{js} %node %t/j 2>&1 | %FileCheck %s --check-prefix=CHECK
// RUN: %run_if{asmjs} %node %t/a 2>&1 | %FileCheck %s --check-prefix=CHECK
// CHECK: JSExport non-trivial destructor 1/1 : SUCCESS

//===---------------------------------------------------------------------===//
//	Copyright 2021 Leaning Technlogies
//===----------------------------------------------------------------------===//

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
		assertEqual(totalInserted() >= uniqueInserted(), true, "JSExport non-trivial destructor 1/1");
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
