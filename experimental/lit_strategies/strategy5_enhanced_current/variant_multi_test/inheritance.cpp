// Test inheritance with jsexport across all targets
// RUN: mkdir -p %t
// RUN: %compile_for{wasm} -O1 -o %t/w %s
// RUN: %compile_for{js} -O1 -o %t/j %s
// RUN: %compile_for{asmjs} -O1 -o %t/a %s
//
// RUN: %run_if{wasm} test -f %t/w && test -f %t/w.wasm
// RUN: %run_if{js} test -f %t/j
// RUN: %run_if{asmjs} test -f %t/a
//
// RUN: %run_if{wasm} %node %t/w 2>&1
// RUN: %run_if{js} %node %t/j 2>&1
// RUN: %run_if{asmjs} %node %t/a 2>&1

//===---------------------------------------------------------------------===//
//	Copyright 2024 Leaning Technologies
//===----------------------------------------------------------------------===//

struct [[cheerp::jsexport]] Interface {
	virtual int pureVirtualTest() = 0;

	virtual ~Interface() {}
};

struct [[cheerp::jsexport]] Base : Interface {
	Base() {}

	int memberBaseTest() { return 1; }
	int memberShadowTest() { return 0; }
	virtual int virtualTest() { return 0; }
	int pureVirtualTest() override { return 0; }

	static int staticBaseTest() { return 1; }
	static int staticShadowTest() { return 0; }
};

struct [[cheerp::jsexport]] Derived : Base {
	Derived() {}

	int memberDerivedTest() { return 1; }
	int memberShadowTest() { return 1; }
	int virtualTest() override { return 1; }
	int pureVirtualTest() override { return 1; }
	
	static int staticDerivedTest() { return 1; }
	static int staticShadowTest() { return 1; }
};
