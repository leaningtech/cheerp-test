// RUN: %cheerp_clang -O1 -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js
// RUN: %cheerp_clang -O1 -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js
// RUN: %cheerp_clang -O1 -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js

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
