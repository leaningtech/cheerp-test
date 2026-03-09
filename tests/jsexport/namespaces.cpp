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
// RUN: regular_only run_if_js python3 %S/../helpers/run_cheerp_module.py --module=es6 %t/es6.mjs 2>&1 | %FileCheck %s
//
// Test CommonJS module
// RUN: regular_only run_if_js compile_mode_js -cheerp-make-module=commonjs -o %t/commonjs.js %s 2>&1
// RUN: regular_only run_if_js python3 %S/../helpers/run_cheerp_module.py --module=commonjs %t/commonjs.js 2>&1 | %FileCheck %s
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

// CHECK-NOT: FAILURE

#include <tests.h>
class [[cheerp::jsexport]][[cheerp::genericjs]] Z
{
public:
	Z(int X) : value(X) {}
	int getValueAndIncrease()
	{
		return value++;
	}
private:
	int value;
};
class [[cheerp::jsexport]][[cheerp::genericjs]] A
{
public:
	A(int X) : value(X) {}
	int getValueAndIncrease()
	{
		return value++;
	}
private:
	int value;
};

[[cheerp::jsexport]]
void emptyFunction()
{
}

namespace B2
{

[[cheerp::jsexport]]
void emptyFunction()
{
}

class [[cheerp::jsexport]][[cheerp::genericjs]] Z
{
public:
	Z(int X) : value(X) {}
	int getValueAndIncrease()
	{
		return value++;
	}
private:
	int value;
};
class [[cheerp::jsexport]][[cheerp::genericjs]] A
{
public:
	A(int X) : value(X) {}
	int getValueAndIncrease()
	{
		return value++;
	}
private:
	int value;
};

class [[cheerp::jsexport]][[cheerp::genericjs]] F
{
public:
	F(int X) : value(X) {}
	int getValueAndIncrease()
	{
		return value++;
	}
private:
	int value;
};

namespace C
{
[[cheerp::jsexport]]
void emptyFunction()
{
}

class [[cheerp::jsexport]][[cheerp::genericjs]] Z
{
public:
	Z(int X) : value(X) {}
	int getValueAndIncrease()
	{
		return value++;
	}
private:
	int value;
};
class [[cheerp::jsexport]][[cheerp::genericjs]] A
{
public:
	A(int X) : value(X) {}
	int getValueAndIncrease()
	{
		return value++;
	}
private:
	int value;
};

namespace D
{

class [[cheerp::jsexport]][[cheerp::genericjs]] A
{
public:
	A(int X) : value(X) {}
	int getValueAndIncrease()
	{
		return value++;
	}
private:
	int value;
};
class [[cheerp::jsexport]][[cheerp::genericjs]] Z
{
public:
	Z(int X) : value(X) {}
	int getValueAndIncrease()
	{
		return value++;
	}
private:
	int value;
};

}//namespace D

}//namespace C

}//namespace B2

namespace B
{

class [[cheerp::jsexport]][[cheerp::genericjs]] Z
{
public:
	Z(int X) : value(X) {}
	int getValueAndIncrease()
	{
		return value++;
	}
private:
	int value;
};
class [[cheerp::jsexport]][[cheerp::genericjs]] A
{
public:
	A(int X) : value(X) {}
	int getValueAndIncrease()
	{
		return value++;
	}
private:
	int value;
};

namespace C
{

class [[cheerp::jsexport]][[cheerp::genericjs]] Z
{
public:
	Z(int X) : value(X) {}
	int getValueAndIncrease()
	{
		return value++;
	}
private:
	int value;
};
class [[cheerp::jsexport]][[cheerp::genericjs]] A
{
public:
	A(int X) : value(X) {}
	int getValueAndIncrease()
	{
		return value++;
	}
private:
	int value;
};

namespace D
{

class [[cheerp::jsexport]][[cheerp::genericjs]] A
{
public:
	A(int X) : value(X) {}
	int getValueAndIncrease()
	{
		return value++;
	}
private:
	int value;
};
class [[cheerp::jsexport]][[cheerp::genericjs]] Z
{
public:
	Z(int X) : value(X) {}
	int getValueAndIncrease()
	{
		return value++;
	}
private:
	int value;
};

}//namespace D

}//namespace C

}//namespace B

[[cheerp::genericjs]]
void webMain()
{
	int ok;
	__asm__("typeof Z === 'function' && typeof A === 'function' && typeof emptyFunction === 'function'" : "=r"(ok) :);
	assertPrint("JSExport namespaces global:", ok);
	// CHECK: JSExport namespaces global: 1

	__asm__(
		"typeof B2 !== 'undefined' && typeof B2.emptyFunction === 'function' && typeof B2.Z === 'function' && typeof B2.A === 'function' && typeof B2.F === 'function'" :
		"=r"(ok) :);
	assertPrint("JSExport namespaces B2:", ok);
	// CHECK: JSExport namespaces B2: 1

	__asm__(
		"typeof B !== 'undefined' && typeof B.C !== 'undefined' && typeof B.C.D !== 'undefined' && typeof B.C.D.A === 'function' && typeof B.C.D.Z === 'function'" :
		"=r"(ok) :);
	assertPrint("JSExport namespaces B.C.D:", ok);
	// CHECK: JSExport namespaces B.C.D: 1
}
