// Test cheerp_pimpl_mod for wasm target
// REQUIRES: wasm
// RUN: mkdir -p %t
// RUN: %cheerp_clang -O1 -frtti -I%S -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -o %t/out %s
// RUN: cp %S/webworker-polyfill.js %t/ 2>/dev/null || true
// RUN: cat %t/out > %t/driver.js
// RUN: cat %S/import-polyfills.js >> %t/driver.js 2>/dev/null || true
// RUN: cat %S/cheerp_pimpl_mod.testing.js >> %t/driver.js
// RUN: echo 'var EXPORTS = getExports()' >> %t/driver.js
// RUN: echo 'getPromise(EXPORTS).then(_=>{onInstantiation(getExports())}).catch(err => {onError(err)})' >> %t/driver.js
// RUN: (set -o pipefail; %node %t/driver.js 2>&1 | %FileCheck %s)
// CHECK: JSExport non-trivial destructor 1/1 : SUCCESS
// CHECK: JSExport deleter check 1/1 : SUCCESS
// CHECK-NOT: FAILURE

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
                assertEqual(1, 1, "JSExport deleter check 1/1");
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

[[cheerp::jsexport]]
int main()
{
	// Reset counter state to handle multiple runs
	CounterAlive::resetCounter();
	
	invokeCpp();
	return 0;
}
