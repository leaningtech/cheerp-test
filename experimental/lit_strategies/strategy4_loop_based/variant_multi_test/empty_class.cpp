// Test empty classes with jsexport across all targets
// RUN: mkdir -p %t
// RUN: %run_all_targets %s %t 2>&1

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
