// Test empty classes with jsexport across all targets
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

class [[cheerp::jsexport]] Empty {};

class [[cheerp::jsexport]] AllPrivate {
	AllPrivate() {}
	void func() { var++; }
	int var;
};

class [[cheerp::jsexport]] OnlyStatic {
public:
	static void func() { var++; }
	static int var;
};

int OnlyStatic::var = 0;

int main() {
	Empty empty;
	OnlyStatic onlyStatic;
}
