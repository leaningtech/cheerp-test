
//===---------------------------------------------------------------------===//
//	Copyright 2020 Leaning Technlogies
//===----------------------------------------------------------------------===//

// This test verifies that jsexport correctly handles complex namespace hierarchies:
// - Classes with identical names in different namespaces don't collide
// - Functions with identical names in different namespaces are properly distinguished
// - Nested namespace structures are correctly represented in JavaScript
// - Both the JavaScript structure (backend) and runtime behavior (end-to-end) are correct

// Test JavaScript structure generation for js target (backend correctness)
// RUN: %if_js %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o - | %FileCheck --check-prefix=JS %s %endif

// Test JavaScript structure generation for asmjs target (backend correctness)
// RUN: %if_asmjs %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o - | %FileCheck --check-prefix=JS %s %endif

// Test runtime behavior for js target (end-to-end correctness)
// RUN: %if_js %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp %s -o %t.js %endif
// RUN: %if_js %node %t.js 2>&1 | %FileCheck --check-prefix=EXEC %s %endif

// Test runtime behavior for asmjs target (end-to-end correctness)
// RUN: %if_asmjs %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm -cheerp-linear-output=asmjs %s -o %t_asmjs.js %endif
// RUN: %if_asmjs %node %t_asmjs.js 2>&1 | %FileCheck --check-prefix=EXEC %s %endif

// Test runtime behavior for wasm target (end-to-end correctness)
// RUN: %if_wasm %cheerp_clang -O1 -frtti -I%S/.. -cheerp-bounds-check -cheerp-fix-wrong-func-casts -target cheerp-wasm %s -o %t_wasm.js %endif
// RUN: %if_wasm %node %t_wasm.js 2>&1 | %FileCheck --check-prefix=EXEC %s %endif

// JS-DAG: function A(
// JS-DAG: function Z(
// JS-DAG: var emptyFunction=
// JS-DAG: B.A=function (
// JS-DAG: B.Z=function (
// JS-DAG: B.C.A=function (
// JS-DAG: B.C.Z=function (
// JS-DAG: B.C.D.A=function (
// JS-DAG: B.C.D.Z=function (
// JS-DAG: B2.A=function (
// JS-DAG: B2.Z=function (
// JS-DAG: B2.F=function (
// JS-DAG: B2.emptyFunction=
// JS-DAG: B2.C.A=function (
// JS-DAG: B2.C.Z=function (
// JS-DAG: B2.C.emptyFunction=
// JS-DAG: B2.C.D.A=function (
// JS-DAG: B2.C.D.Z=function (
// JS-DAG: A.prototype.getValueAndIncrease=function()
// JS-DAG: Z.prototype.getValueAndIncrease=function()
// JS-DAG: B.A.prototype.getValueAndIncrease=function()
// JS-DAG: B.Z.prototype.getValueAndIncrease=function()
// JS-DAG: B2.A.prototype.getValueAndIncrease=function()
// JS-DAG: B2.Z.prototype.getValueAndIncrease=function()
// JS-DAG: B2.F.prototype.getValueAndIncrease=function()

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
	// Create instances from all namespaces to verify they are distinct and functional
	Z z1(1);
	A a1(2);
	
	B::Z bz(3);
	B::A ba(4);
	B::C::Z bcz(5);
	B::C::A bca(6);
	B::C::D::Z bcdz(7);
	B::C::D::A bcda(8);
	
	B2::Z b2z(9);
	B2::A b2a(10);
	B2::F b2f(11);
	B2::C::Z b2cz(12);
	B2::C::A b2ca(13);
	B2::C::D::Z b2cdz(14);
	B2::C::D::A b2cda(15);
	
	// Verify methods work correctly and return expected values
	assertEqual(z1.getValueAndIncrease(), 1, "Global Z");
	assertEqual(a1.getValueAndIncrease(), 2, "Global A");
	
	assertEqual(bz.getValueAndIncrease(), 3, "B::Z");
	assertEqual(ba.getValueAndIncrease(), 4, "B::A");
	assertEqual(bcz.getValueAndIncrease(), 5, "B::C::Z");
	assertEqual(bca.getValueAndIncrease(), 6, "B::C::A");
	assertEqual(bcdz.getValueAndIncrease(), 7, "B::C::D::Z");
	assertEqual(bcda.getValueAndIncrease(), 8, "B::C::D::A");
	
	assertEqual(b2z.getValueAndIncrease(), 9, "B2::Z");
	assertEqual(b2a.getValueAndIncrease(), 10, "B2::A");
	assertEqual(b2f.getValueAndIncrease(), 11, "B2::F");
	assertEqual(b2cz.getValueAndIncrease(), 12, "B2::C::Z");
	assertEqual(b2ca.getValueAndIncrease(), 13, "B2::C::A");
	assertEqual(b2cdz.getValueAndIncrease(), 14, "B2::C::D::Z");
	assertEqual(b2cda.getValueAndIncrease(), 15, "B2::C::D::A");
	
	// Call all emptyFunction variants to ensure they exist and don't collide
	emptyFunction();
	B2::emptyFunction();
	B2::C::emptyFunction();
	
	client::console.log("All namespace tests completed");
}

// EXEC: Global Z : SUCCESS
// EXEC: Global A : SUCCESS
// EXEC: B::Z : SUCCESS
// EXEC: B::A : SUCCESS
// EXEC: B::C::Z : SUCCESS
// EXEC: B::C::A : SUCCESS
// EXEC: B::C::D::Z : SUCCESS
// EXEC: B::C::D::A : SUCCESS
// EXEC: B2::Z : SUCCESS
// EXEC: B2::A : SUCCESS
// EXEC: B2::F : SUCCESS
// EXEC: B2::C::Z : SUCCESS
// EXEC: B2::C::A : SUCCESS
// EXEC: B2::C::D::Z : SUCCESS
// EXEC: B2::C::D::A : SUCCESS
// EXEC: All namespace tests completed
