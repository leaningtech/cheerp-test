//===----------------------------------------------------------------------===//
//      Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//
// Test 64-bit integer arithmetics

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -cheerp-use-bigints -o %t/j %s 2>&1 && node %t/j 2>&1 | %FileCheck --check-prefixes=CHECK,CHECK-REG %s
// RUN: regular_only run_if_wasm compile_mode_wasm -cheerp-use-bigints -o %t/w %s 2>&1 && node %t/w 2>&1 | %FileCheck --check-prefixes=CHECK,CHECK-REG %s
// RUN: regular_only run_if_asmjs compile_mode_asmjs -cheerp-use-bigints -o %t/a %s 2>&1 && node %t/a 2>&1 | %FileCheck --check-prefixes=CHECK,CHECK-REG %s
// RUN: preexec_only run_if_js compile_mode_js -cheerp-use-bigints -o %t/j %s 2>&1 | %FileCheck %s
// RUN: preexec_only run_if_asmjs compile_mode_asmjs -cheerp-use-bigints -o %t/a %s 2>&1 | %FileCheck %s

#include <tests.h>
#include <stdarg.h>

// TODO: fix dump output

// Test 64-bit integer arithmetics
#ifndef PRE_EXECUTE_TEST
template <typename T>
static void dump(const T& t) {
	long h = (long)(t >> 32);
	long l = (long)(t & 0xffffffff);
	assertPrint("highint h:", Hex(h));
	assertPrint("highint l:", Hex(l));
	// cheerp::console_log("highint h:", h, "l:", l);
}
#endif

void testRepresentation() {
	assertPrint("int64_t representation 1/N:", Hex((unsigned long long)0));
	assertPrint("int64_t representation 2/N:", Hex((signed long long)-1));
	assertPrint("int64_t representation 3/N:", Hex((unsigned long long)-1));
	assertPrint("int64_t representation 4/N:", Hex(9223372036854775807));
}

template <typename T>
static void testShiftOps() {
	T a = unitBlackBox(0x10000);
	T b = unitBlackBox(0x100000000);
	T c = unitBlackBox(0x1000000000000);
	T d = unitBlackBox(0x7fffffffLL);
	T e = unitBlackBox(0x7fffffff00000000LL);

	assertPrint("int64_t shl support 1/N:", Hex(a << 0));
	assertPrint("int64_t shl support 2/N:", Hex(a << 8));
	assertPrint("int64_t shl support 3/N:", Hex(a << 16));
	assertPrint("int64_t shl support 4/N:", Hex(a << 32)); 
	assertPrint("int64_t shl support 5/N:", Hex(d << 32)); 

	assertPrint("int64_t shr support 1/N:", Hex(a >> 0));
	assertPrint("int64_t shr support 2/N:", Hex(a >> 8));
	assertPrint("int64_t shr support 3/N:", Hex(b >> 16));
	assertPrint("int64_t shr support 4/N:", Hex(c >> 32));
	assertPrint("int64_t shr support 5/N:", Hex(e >> 32));

	if (std::is_unsigned<T>::value) {
		T f = unitBlackBox(0xffffffff);
		T g = unitBlackBox(0xffffffff00000000);
		assertPrint("int64_t shr support 6/N:", (g >> 32) == f);
		// assertEqual(g >> 32, f, "int64_t shr support 6a/N");
	} else {
		T f = unitBlackBox(0xffffffffffffffff);
		T g = unitBlackBox(0xffffffff00000000);
		// assertEqual(g >> 32, f, "int64_t shr support 11/N");
		assertPrint("int64_t shr support 6/N:", (g >> 32) == f);
	}
}

template <typename T>
static void testBitwiseOps() {
	T a = unitBlackBox(0xffffffffffffffffLL);
	T b = unitBlackBox(0x0000000000000000LL);
	T l = unitBlackBox(0x00000000ffffffffLL);
	T u = unitBlackBox(0xffffffff00000000LL);

	assertPrint("int64_t AND support 1/N:", (a & l) == l);
	assertPrint("int64_t AND support 2/N:", (a & u) == u);

	assertPrint("int64_t OR support 1/N:", (a | l) == a);
	assertPrint("int64_t OR support 2/N:", (b | l) == l);
	assertPrint("int64_t OR support 3/N:", (a | u) == a);
	assertPrint("int64_t OR support 4/N:", (b | u) == u);

	assertPrint("int64_t XOR support 1/N:", (a ^ b) == a);
	assertPrint("int64_t XOR support 2/N:", (a ^ a) == b);
	assertPrint("int64_t XOR support 3/N:", (b ^ b) == b);

	assertPrint("int64_t NOT support 1/N:", (~a) == b);
	assertPrint("int64_t NOT support 2/N:", (~b) == a);
	assertPrint("int64_t NOT support 3/N:", (~l) == u);
}

template <typename T>
static void testAddSubOps() {
	T a = unitBlackBox(0x0deadbeaf);
	T b = unitBlackBox(0x100000000);
	T c = unitBlackBox(0x0ffffffff);
	T d = unitBlackBox(0xffffffffffffffff);
	T e = unitBlackBox(0xfffffffffffffffe);

	assertPrint("int64_t add support 1/N:", Hex(a + b));
	assertPrint("int64_t add support 2/N:", Hex(c + 1));
	assertPrint("int64_t add support 3/N:", (d + 1) == 0x000000000);
	assertPrint("int64_t add support 4/N:", (d + d) == 0xfffffffffffffffe);

	assertPrint("int64_t sub support 1/N:", Hex(a - 1));
	assertPrint("int64_t sub support 2/N:", Hex(b - 1));
	assertPrint("int64_t sub support 3/N:", Hex(b - b));
	assertPrint("int64_t sub support 4/N:", (c - b) == 0xffffffffffffffff);
	assertPrint("int64_t sub support 5/N:", Hex(d - d));
	T f = -1;
	assertPrint("int64_t sub support 6/N:", (f + -1) == 0xfffffffffffffffe);
}

template <typename T>
static void testMulDivModOps() {
	T a = unitBlackBox(0x00018001);
	// assertEqual(a * 2, (T)0x00030002, "int64_t mul support 1/N");
	assertPrint("int64_t mul support 1/N:", (a * 2) == (T)0x00030002);

	T b = unitBlackBox(0x0000000088888888);
	// assertEqual(b * 2, (T)0x0000000111111110, "int64_t mul support 2/N");
	assertPrint("int64_t mul support 2/N:", (b * 2) == (T)0x0000000111111110);


	T c = unitBlackBox(0x1111111110000000);
	assertPrint("int64_t mul support 3/N:", c * 2 == (T)0x2222222220000000);
	// assertEqual(c * 2, (T)0x2222222220000000, "int64_t mul support 3/N");

	T d = unitBlackBox(0x1122334455667788);
	T e = unitBlackBox(0x1111111111111111);
	// assertEqual(d * e, (T)0xcba862fb71c5f808, "int64_t mul support 4/N");
	assertPrint("int64_t mul support 4/N:", (d * e) == (T)0xcba862fb71c5f808);
 
	T f = unitBlackBox(0x7fffffff);
	assertPrint("int64_t mul support 5/N:", (f * f) == (T)0x3fffffff00000001);
	// assertEqual(f * f, unitBlackBox((T)0x3fffffff00000001), "int64_t mul support 5/N");

	// assertEqual(f / f, (T)1, "int64_t div support 1/N");
	assertPrint("int64_t div support 1/N:", f / f);
	// assertEqual(d / d, (T)1, "int64_t div support 2/N");
	assertPrint("int64_t div support 2/N:", d / d);
	if (std::is_unsigned<T>::value) {
		assertPrint("int64_t div support 3/N:", (T)((-d) / d) == (T)13);
		// assertEqual((T)((-d) / d), (T)13, "int64_t div support 3a/N");
	} else {
		assertPrint("int64_t div support 3/N:", (T)((-d) / d) == (T)-1);
		// assertEqual((T)((-d) / d), (T)-1, "int64_t div support 3b/N");
	}
	T g = 0xf;
	assertPrint("int64_t div support 4/N:", g / 3 == (T)5);
	// assertEqual(g / 3, (T)5, "int64_t div support 4/N");

	// assertEqual(f % f, (T)0, "int64_t mod support 1/N");
	assertPrint("int64_t mod support 1/N:", (f % f) == (T)0);
	// assertEqual((T)(-1) % 1, (T)0, "int64_t mod support 2/N");
	assertPrint("int64_t mod support 2/N:", (T)(-1) % 1 == (T)0);
	// assertEqual(d % e, (T)0x0011223344556677, "int64_t mod support 3/N");
	assertPrint("int64_t mod support 3/N:", d % e == (T)0x0011223344556677);
}

template <typename T>
static void testUnaryOps() {
	T a = unitBlackBox(0x0);
	T b = unitBlackBox(0x1);
	T c = unitBlackBox(0x0000000100000000);
	T d = unitBlackBox(0xffffffffffffffff);
	T e = unitBlackBox(0x1122334455667788);

	// assertEqual(-a, (T)0, "int64_t neg support 1/N");
	assertPrint("int64_t neg support 1/N:", (-a == (T)0));
	// assertEqual(-b, (T)0xffffffffffffffff, "int64_t neg support 2/N");
	assertPrint("int64_t neg support 2/N:", (-b == (T)0xffffffffffffffff));
	// assertEqual(-c, (T)0xffffffff00000000, "int64_t neg support 3/N");
	assertPrint("int64_t neg support 3/N:", (-c == ((T)0xffffffff00000000)));
	// assertEqual(-d, (T)0x00000001, "int64_t neg support 4/N");
	assertPrint("int64_t neg support 4/N:", (-d == (T)0x00000001));
	// assertEqual(-e, (T)0xeeddccbbaa998878, "int64_t neg support 5/N");
	assertPrint("int64_t neg support 5/N:", (-e == (T)0xeeddccbbaa998878));
	// assertEqual(-0x7fffffffffffffff == 0x8000000000000001, true, "int64_t neg support 6a/N");
	assertPrint("int64_t neg support 6/N:", (-0x7fffffffffffffff == 0x8000000000000001));

	// assertEqual(!a, true, "int64_t lnot support 1/N");
	assertPrint("int64_t lnot support 1/N:", !a);
	// assertEqual(!b, false, "int64_t lnot support 2/N");
	assertPrint("int64_t lnot support 2/N:", !b);
	// assertEqual(!c, false, "int64_t lnot support 3/N");
	assertPrint("int64_t lnot support 3/N:", !c);
	// assertEqual(!d, false, "int64_t lnot support 4/N");
	assertPrint("int64_t lnot support 4/N:", !d);
}

template <typename T>
static void testBitwiseCompoundAssignmentOps() {
	T a = unitBlackBox(0x0);
	T b = unitBlackBox(0x1);
	T c = unitBlackBox(0x0000000100000000);
	T d = unitBlackBox(0xffffffffffffffff);
	T t;

	t = a; t &= b;
	assertPrint("int64_t and assign support 1/N:", Hex(t));
	// assertEqual(t, (T)0x0000000000000000, "int64_t and assign support 1/N");
	t = b; t &= c;
	// assertEqual(t, (T)0x0000000000000000, "int64_t and assign support 2/N");
	assertPrint("int64_t and assign support 2/N:", Hex(t));	
	t = c; t &= d;
	// assertEqual(t, (T)0x0000000100000000, "int64_t and assign support 3/N");
	assertPrint("int64_t and assign support 3/N:", Hex(t));	
	t = d; t &= a;
	assertPrint("int64_t and assign support 4/N:", Hex(t));		
	// assertEqual(t, (T)0x0000000000000000, "int64_t and assign support 4/N");

	t = a; t |= b;
	assertPrint("int64_t or assign support 1/N:", Hex(t));
	// assertEqual(t, (T)0x0000000000000001, "int64_t or assign support 1/N");
	t = b; t |= c;
	assertPrint("int64_t or assign support 2/N:", Hex(t));
	// assertEqual(t, (T)0x0000000100000001, "int64_t or assign support 2/N");
	t = c; t |= d;
	assertPrint("int64_t or assign support 3/N:", t == (T)0xffffffffffffffff);
	// assertEqual(t, (T)0xffffffffffffffff, "int64_t or assign support 3/N");
	t = d; t |= a;
	assertPrint("int64_t or assign support 4/N:", t == (T)0xffffffffffffffff);
	// assertEqual(t, (T)0xffffffffffffffff, "int64_t or assign support 4/N");

	t = a; t ^= b;
	assertPrint("int64_t xor assign support 1/N:", Hex(t));
	// assertEqual(t, (T)0x0000000000000001, "int64_t xor assign support 1/N");
	t = b; t ^= c;
	assertPrint("int64_t xor assign support 2/N:", Hex(t));
	// assertEqual(t, (T)0x0000000100000001, "int64_t xor assign support 2/N");
	t = c; t ^= d;
	assertPrint("int64_t xor assign support 3/N:", (t == (T)0xfffffffeffffffff));
	// assertEqual(t, (T)0xfffffffeffffffff, "int64_t xor assign support 3/N");
	t = d; t ^= a;
	assertPrint("int64_t xor assign support 4/N:", (t == (T)0xffffffffffffffff));
	// assertEqual(t, (T)0xffffffffffffffff, "int64_t xor assign support 4/N");
}

template <typename T>
static void testArithmeticCompoundAssignmentOps() {
	T a = unitBlackBox(0x0);
	T b = unitBlackBox(0x1);
	T c = unitBlackBox(0x0000000100000000);
	T d = unitBlackBox(0xffffffffffffffff);
	T t;

	t = a; t += b;
	// assertEqual(t, (T)0x0000000000000001, "int64_t add assign support 1/N");
	assertPrint("int64_t add assign support 1/N:", Hex(t));
	t = b; t += c;
	// assertEqual(t, (T)0x0000000100000001, "int64_t add assign support 2/N");
	assertPrint("int64_t add assign support 2/N:", Hex(t));
	t = c; t += d;
	// assertEqual(t, (T)0x00000000ffffffff, "int64_t add assign support 3/N");
	assertPrint("int64_t add assign support 3/N:", Hex(t));
	t = d; t += a;
	// assertEqual(t, (T)0xffffffffffffffff, "int64_t add assign support 4/N");
	assertPrint("int64_t add assign support 4/N:", (t == (T)0xffffffffffffffff));

	t = a; t -= b;
	assertPrint("int64_t sub assign support 1/N:", t == (T)0xffffffffffffffff);
	// assertEqual(t, (T)0xffffffffffffffff, "int64_t sub assign support 1/N");
	t = b; t -= c;
	// assertEqual(t, (T)0xffffffff00000001, "int64_t sub assign support 2/N");
	assertPrint("int64_t sub assign support 2/N:", t == (T)0xffffffff00000001);
	t = c; t -= d;
	// assertEqual(t, (T)0x0000000100000001, "int64_t sub assign support 3/N");
	assertPrint("int64_t sub assign support 3/N:", t == (T)0x0000000100000001);
	t = d; t -= a;
	assertPrint("int64_t sub assign support 4/N:", t == (T)0xffffffffffffffff);
	// assertEqual(t, (T)0xffffffffffffffff, "int64_t sub assign support 4/N");

	t = a; t %= b;
	assertPrint("int64_t mod assign support 1/N:", Hex(t));
	// assertEqual(t, (T)0x0000000000000000, "int64_t mod assign support 1/N");
	t = b; t %= c;
	assertPrint("int64_t mod assign support 2/N:", Hex(t));
	// assertEqual(t, (T)0x0000000000000001, "int64_t mod assign support 2/N");
	t = c; t %= d;
	if (std::is_unsigned<T>::value) {
		assertPrint("int64_t mod assign support 3/N:", (t == 0x0000000100000000));
		// assertEqual(t, (T)0x0000000100000000, "int64_t mod assign support 3a/N");
	} else {
		assertPrint("int64_t mod assign support 3/N:", (t == 0x0000000000000000));
		// assertEqual(t, (T)0x0000000000000000, "int64_t mod assign support 3b/N");
	}
	t = d; t %= b;
	// assertEqual(t, (T)0x0000000000000000, "int64_t mod assign support 4/N");
	assertPrint("int64_t mod assign support 4/N:", Hex(t));

	t = a; t *= b;
	assertPrint("int64_t mul assign support 1/N:", Hex(t));
	// assertEqual(t, (T)0x0000000000000000, "int64_t mul assign support 1/N");
	t = b; t *= c;
	assertPrint("int64_t mul assign support 2/N:", Hex(t));
	// assertEqual(t, (T)0x0000000100000000, "int64_t mul assign support 2/N");
	t = c; t *= d;
	// assertEqual(t, -c, "int64_t mul assign support 3/N");
	assertPrint("int64_t mul assign support 3/N:", (t == -c));
	t = d; t *= b;
	assertPrint("int64_t mul assign support 4/N:", t == 0xffffffffffffffff);
	// assertEqual(t, (T)0xffffffffffffffff, "int64_t mul assign support 4/N");

	t = a; t /= b;
	// assertEqual(t, (T)0x0000000000000000, "int64_t div assign support 1/N");
	assertPrint("int64_t div assign support 1/N:", Hex(t));
	t = b; t /= c;
	assertPrint("int64_t div assign support 2/N:", Hex(t));
	// assertEqual(t, (T)0x0000000000000000, "int64_t div assign support 2/N");
	t = c; t /= d;
	if (std::is_unsigned<T>::value) {
		assertPrint("int64_t mod assign support 3/N:", (t == (T)0x0000000000000000));
		// assertEqual(t, (T)0x0000000000000000, "int64_t mod assign support 3/N");
	} else {
		assertPrint("int64_t mod assign support 3/N:", (t == -c));
		// assertEqual(t, -c, "int64_t mod assign support 3/N");
	}
	t = d; t /= b;
	assertEqual(t, (T)0xffffffffffffffff, "int64_t div assign support 4/N");
	assertPrint("int64_t div assign support 4/N:", (t == (T)0xffffffffffffffff));

	t = b; t <<= 0;
	// assertEqual(t, (T)0x0000000000000001, "int64_t shl assign support 1/N");
	assertPrint("int64_t shl assign support 1/N:", Hex(t)); 
	t = c; t <<= 16;
	assertPrint("int64_t shl assign support 2/N:", t == (T)0x0001000000000000);
	// assertEqual(t, (T)0x0001000000000000, "int64_t shl assign support 2/N");
	t = d; t <<= 32;
	assertPrint("int64_t shl assign support 3/N:", t == (T)0xffffffff00000000);
	// assertEqual(t, (T)0xffffffff00000000, "int64_t shl assign support 3/N");

	t = b; t >>= 0;
	// assertEqual(t, (T)0x0000000000000001, "int64_t shr assign support 1/N");
	assertPrint("int64_t shr assign support 1/N:", Hex(t));
	t = c; t >>= 16;
	// assertEqual(t, (T)0x0000000000010000, "int64_t shr assign support 2/N");
	assertPrint("int64_t shr assign support 2/N:", Hex(t));
	t = d; t >>= 32;
	if (std::is_unsigned<T>::value) {
		assertPrint("int64_t shr assign support 3/N:", (t == (T)0x00000000ffffffff));
		// assertEqual(t, (T)0x00000000ffffffff, "int64_t shr assign support 3a/N");
	} else {
		// assertEqual(t, (T)0xffffffffffffffff, "int64_t shr assign support 3b/N");
		assertPrint("int64_t shr assign support 3/N:", (t == (T)0xffffffffffffffff)); 
	}

	T s;
	volatile T r;
	t = a; r = t += a;
	s = r;
	// assertEqual(t, s, "int64_t compound assign result support");
	assertPrint("int64_t compound assign result support:", (t == s));

}

template <typename T>
static void testComparisonOps() {
	T a = unitBlackBox(0x0);
	T b = unitBlackBox(0x1);
	T c = unitBlackBox(0x0000000100000000);
	T e = unitBlackBox(0xffffffffffffffff);
	T t = unitBlackBox(a);
	T h = unitBlackBox(0xf000000000000000);

	// assertEqual(a < b, true, "int64_t lt support 1/N");
	assertPrint("int64_t lt support 1/N:", a < b);
	// assertEqual(a < c, true, "int64_t lt support 2/N");
	assertPrint("int64_t lt support 2/N:", a < c);
	assertPrint("int64_t lt support 3/N:", std::is_signed<T>::value == (e < a));
	// assertEqual(e < a, std::is_signed<T>::value, "int64_t lt support 3/N");
	// assertEqual(e < c, std::is_signed<T>::value, "int64_t lt support 4/N");
	assertPrint("int64_t lt support 4/N:", std::is_signed<T>::value == (e < c));
	// assertEqual(b < c, true, "int64_t lt support 5/N");
	assertPrint("int64_t lt support 5/N:", (b < c));
	// assertEqual(c < b, false, "int64_t lt support 6/N");
	assertPrint("int64_t lt support 6/N:", (c < b));
	assertPrint("int64_t lt support 7/N:", (t < a));
	// assertEqual(t < a, false, "int64_t lt support 7/N");
	assertPrint("int64_t lt support 8/N:", (std::is_signed<T>::value == (h < 0)));
	// assertEqual(h < 0, std::is_signed<T>::value, "int64_t lt support 8/N");

	assertPrint("int64_t le support 1/N:", a <= b);
	assertPrint("int64_t le support 2/N:", a <= c);
	assertPrint("int64_t le support 3/N:", (e <= a) == std::is_signed<T>::value);
	assertPrint("int64_t le support 4/N:", (e <= c) == std::is_signed<T>::value);
	assertPrint("int64_t le support 5/N:", b <= c);
	assertPrint("int64_t le support 6/N:", c <= b);
	assertPrint("int64_t le support 7/N:", t <= a);
	assertPrint("int64_t le support 8/N:", (h <= 0) == std::is_signed<T>::value);

	// assertEqual(a <= b, true, "int64_t le support 1/N");
	// assertEqual(a <= c, true, "int64_t le support 2/N");
	// assertEqual(e <= a, std::is_signed<T>::value, "int64_t le support 3/N");
	// assertEqual(e <= c, std::is_signed<T>::value, "int64_t le support 4/N");
	// assertEqual(b <= c, true, "int64_t le support 5/N");
	// assertEqual(c <= b, false, "int64_t le support 6/N");
	// assertEqual(t <= a, true, "int64_t le support 7/N");
	// assertEqual(h <= 0, std::is_signed<T>::value, "int64_t le support 8/N");

	assertPrint("int64_t gt support 1/N:", (a > b));
	assertPrint("int64_t gt support 2/N:", (a > c));
	assertPrint("int64_t gt support 3/N:", (e > a) == std::is_unsigned<T>::value);
	assertPrint("int64_t gt support 4/N:", (e > c) == std::is_unsigned<T>::value);
	assertPrint("int64_t gt support 5/N:", (b > c));
	assertPrint("int64_t gt support 6/N:", (c > b));
	assertPrint("int64_t gt support 7/N:", (t > a));
	assertPrint("int64_t gt support 8/N:", (h > 0) == std::is_unsigned<T>::value);

	// assertEqual(a > b, false, "int64_t gt support 1/N");
	// assertEqual(a > c, false, "int64_t gt support 2/N");
	// assertEqual(e > a, std::is_unsigned<T>::value, "int64_t gt support 3/N");
	// assertEqual(e > c, std::is_unsigned<T>::value, "int64_t gt support 4/N");
	// assertEqual(b > c, false, "int64_t gt support 5/N");
	// assertEqual(c > b, true, "int64_t gt support 6/N");
	// assertEqual(t > a, false, "int64_t gt support 7/N");
	// assertEqual(h > 0, std::is_unsigned<T>::value, "int64_t gt support 8/n");

	assertPrint("int64_t ge support 1/N:", (a >= b));
	assertPrint("int64_t ge support 2/N:", (a >= c));
	assertPrint("int64_t ge support 3/N:", (e >= a) == std::is_unsigned<T>::value);
	assertPrint("int64_t ge support 4/N:", (e >= c) == std::is_unsigned<T>::value);
	assertPrint("int64_t ge support 5/N:", (b >= c));
	assertPrint("int64_t ge support 6/N:", (c >= b));
	assertPrint("int64_t ge support 7/N:", (t >= a));
	assertPrint("int64_t ge support 8/N:", (h >= 0) == std::is_unsigned<T>::value);

	// assertEqual(a >= b, false, "int64_t ge support 1/N");
	// assertEqual(a >= c, false, "int64_t ge support 2/N");
	// assertEqual(e >= a, std::is_unsigned<T>::value, "int64_t ge support 3/N");
	// assertEqual(e >= c, std::is_unsigned<T>::value, "int64_t ge support 4/N");
	// assertEqual(b >= c, false, "int64_t ge support 5/N");
	// assertEqual(c >= b, true, "int64_t ge support 6/N");
	// assertEqual(t >= a, true, "int64_t ge support 7/N");
	// assertEqual(h >= 0, std::is_unsigned<T>::value, "int64_t ge support 8/n");

	bool zeroIsFalse = true;
	if(a)
		zeroIsFalse = false;
	// assertEqual(zeroIsFalse, true, "int64_t zero is false 1/N");
	assertPrint("int64_t zero is false 1/N:", zeroIsFalse);
}

template <typename T>
static void testCastToFloat() {
	T a = unitBlackBox(0x0);
	T b = unitBlackBox(0x1);
	T c = unitBlackBox(0x0000000100000000);
	T e = unitBlackBox(0xffffffffffffffff);

	assertPrint("int64_t cast to float support 1/N:", (float)a >= (0.f - 1e-6) && (float)a <= (0.f + 1e-6));
	// assertEqual((float) a, 0.f, 1e-6, "int64_t cast to float support 1/N");
	assertPrint("int64_t cast to float support 2/N:", (float)b >= (1.f - 1e-6) && (float)b <= (1.f + 1e-6));
	// assertEqual((float) b, 1.f, 1e-6, "int64_t cast to float support 2/N");
	assertPrint("int64_t cast to float support 3/N:", (float)c >= (4.2949673e9f - 1e-6) && (float)c <= (4.2949673e9f + 1e-6));
	// assertEqual((float) c, 4.2949673e9f, 1e-6, "int64_t cast to float support 3/N");

	T h = 9223372036854775807LL;
	// assertEqual((double) h, 9.223372036854776e18, 1e-10, "int64_t cast to float support 4/N");
	assertPrint("int64_t cast to float support 4/N:", ((double) h >= (9.223372036854776e18 - 1e-10) && ((double) h <= (9.223372036854776e18 + 1e-10))));

	if (std::is_unsigned<T>::value) {
		assertPrint("int64_t cast to float support 5/N:", ((double)e >= (18446744073709551616. - 1e4) && ((double)e <= (18446744073709551616. + 1e4))));
		// assertEqual((double) e, 18446744073709551616., 1e4, "int64_t cast to float support 5a/N");
	} else {
		// assertEqual((double) e, -1., 1e-6, "int64_t cast to float support 5b/N");
		assertPrint("int64_t cast to float support 6/N:", ((double)e >= (-1. - 1e-6) && ((double)e < (-1. + 1e-6))));
		T d = 0x8000000000000000;
		assertPrint("int64_t cast to float support 7/N:", ((double)d >= (-9223372036854776000. - 1e-1) && ((double)d <= (-9223372036854776000. + 1e-1))));
		// assertEqual((double) d, -9223372036854776000., 1e-1, "int64_t cast to float support 6b/N");
		T f = 0x7fffffffffffffff;
		assertPrint("int64_t cast to float support 8/N:", ((double)f >= (9223372036854775808. - 1e-1) && (double)f <= (9223372036854775808. + 1e-1)));
		// assertEqual((double) f, 9223372036854775808., 1e-1, "int64_t cast to float support 7b/N");
	}
}

template <typename T>
static void testCastFromFloat() {
	float a = unitBlackBox(0);
	float b = unitBlackBox(1);
	float c = unitBlackBox(1337.);
	float d = unitBlackBox(3.142);
	float e = unitBlackBox(-1.);

	// assertEqual((T) a, (T)0, "int64_t cast from float support 1/N");
	assertPrint("int64_t cast from float support 1/N:", (T)a);
	// assertEqual((T) b, (T)1, "int64_t cast from float support 2/N");
	assertPrint("int64_t cast from float support 2/N:", (T)b);
    // assertEqual((T) c, (T)1337, "int64_t cast from float support 3/N");
	assertPrint("int64_t cast from float support 3/N:", (T)c);
	// assertEqual((T) d, (T)3, "int64_t cast from float support 4/N");
	assertPrint("int64_t cast from float support 4/N:", (T)d);

	if (std::is_signed<T>::value) {
		assertPrint("int64_t cast from float support 5/N:", (T) e);
		// assertEqual((T) e, (T)-1, "int64_t cast from float support 5/N");

		float f = unitBlackBox(-3.2e3);
				assertPrint("int64_t cast from float support 6/N:", (T) f);
		// assertEqual((T) f, (T)-3200, "int64_t cast from float support 6/N");

		float g = unitBlackBox(-3.2e9);
		assertPrint("int64_t cast from float support 7/N:", (T) g);
		// assertEqual((T) g, (T)-3200000000, "int64_t cast from float support 7/N");

		T h = unitBlackBox(0x7ffffffffffff000);
		double i = unitBlackBox(9223372036854771712.);
		assertPrint("int64_t cast from float support 8/N:", (T) h);
		// assertEqual((T) i, h, "int64_t cast from float support 8/N");
	}
}

template <typename T>
static void testIncrement() {
	T a = unitBlackBox(0);	
	a++;
	assertPrint("int64_t post increment support 1/3:", a == 1);

	// assertEqual(a, (T)1, "int64_t post increment support 1/N");
	a = unitBlackBox(0x00000000ffffffff);
	assertPrint("int64_t post increment support 2/3:", Hex((T)a++));
	assertPrint("int64_t post increment support 3/3:", Hex((T)a));
	// assertEqual(a++, (T)0x00000000ffffffff, "int64_t post increment support 2/N");
	// assertEqual(a, (T)0x0000000100000000, "int64_t post increment support 3/N");

	T b = unitBlackBox(0);
	++b;
	assertPrint("int64_t pre increment support 1/3:", Hex((T)b)); 
	// assertEqual(b, (T)1, "int64_t pre increment support 1/N");
	b = unitBlackBox(0x00000000ffffffff);
	assertPrint("int64_t pre increment support 2/3:", Hex((T)++b));
	// assertEqual(++b, (T)0x0000000100000000, "int64_t pre increment support 2/N");
	assertPrint("int64_t pre increment support 3/3:", Hex((T)b));
	// assertEqual(b, (T)0x0000000100000000, "int64_t pre increment support 3/N");

	a = unitBlackBox(0);
	a--;
	assertPrint("int64_t post decrement support 1/3:", Hex((T)a));
	// assertEqual(a, (T)0xffffffffffffffff, "int64_t post decrement support 1/N");
	a = unitBlackBox(0xffffffffffffffff);
	assertPrint("int64_t post decrement support 2/3:", Hex((T)a--));
	assertPrint("int64_t post decrement support 3/3:", Hex((T)a));
	// assertEqual(a--, (T)0xffffffffffffffff, "int64_t post decrement support 2/N:");
	// assertEqual(a, (T)0xfffffffffffffffe, "int64_t post decrement support 3/N:");

	b = unitBlackBox(0);
	--b;
	assertPrint("int64_t pre decrement support 1/3:", Hex((T)b));
	// assertEqual(b, (T)0xffffffffffffffff, "int64_t pre decrement support 1/N:");
	b = unitBlackBox(0xffffffffffffffff);
	assertPrint("int64_t pre decrement support 2/3:", Hex((T)--b));
	assertPrint("int64_t pre decrement support 3/3:", Hex((T)b));
	// assertEqual(--b, (T)0xfffffffffffffffe, "int64_t pre decrement support 2/N:");
	// assertEqual(b, (T)0xfffffffffffffffe, "int64_t pre decrement support 3/N:");
}

template <typename T>
static void testStructs() {
	typedef struct s_ {
		T a;
		T b;
		int c;
	} s;

	s foo;
	foo.a = unitBlackBox(42);
	foo.b = unitBlackBox(0x7fffffffffffffff);
	foo.c = unitBlackBox(1);

	assertPrint("int64_t struct support 1/3:", (foo.a < foo.b));
	assertPrint("int64_t struct support 2/3:", (foo.a > foo.c));
	assertPrint("int64_t struct support 3/3:", (foo.b > foo.c));
	// assertEqual(foo.a < foo.b, true, "int64_t struct support 1/N");
	// assertEqual(foo.a > foo.c, true, "int64_t struct support 2/N");
	// assertEqual(foo.b > foo.c, true, "int64_t struct support 3/N");
}

template <typename T>
static void increment(T& a) {
	a += unitBlackBox(1);
}

template <typename T>
static void testPointerArithmetic() {
	T *a = new T[20];
	memset(a, unitBlackBox(0), sizeof(T)*20);
	bool valid = true;
	for (int i = 0; i < 20; i++)
		valid &= (a[i] == 0);
	assertPrint("int64_t list support 1/2:", valid);
	// assertEqual(valid, true, "int64_t list support 1/N");

	for (int i = 0; i < 20; i++) {
		a[i] = ((0xffffffffLL - i) << 32) + i;
	}

	valid = true;
	for (int i = 0; i < 20; i++)
		valid &= (a[i] != 0);
	assertPrint("int64_t list support 2/2:", valid);
	// assertEqual(valid, true, "int64_t list support 2/N");
}

template <typename T>
static void testPointersAndReferences() {
	T a = unitBlackBox(42);
	increment<T>(a);
	// assertEqual(a, (T)43, "int64_t reference support 1/N");
	assertPrint("int64_t reference support 1/N:", a); 
}

static void testBitfields() {
	struct A {
		unsigned long long a : 3;
		unsigned long long b : 3;
	};
	struct A foo;
	foo.a = unitBlackBox(7);
	foo.b = unitBlackBox(4);
	// assertEqual(foo.a, 7ULL, "int64_t bit fields support 1/N");
	assertPrint("int64_t bit fields support 1/N:", foo.a);
	// assertEqual(foo.b, 4ULL, "int64_t bit fields support 2/N");
	assertPrint("int64_t bit fields support 2/N:", foo.b);

	// Test 0 bit field
	struct B {
		unsigned long long a1 : 3;
		unsigned long long a2 : 3;
		unsigned long long : 0;
		unsigned long long b1 : 3;
		unsigned long long b2 : 3;
	};
	struct B bar;
	bar.a1 = unitBlackBox(7);
	bar.a2 = unitBlackBox(4);
	bar.b1 = unitBlackBox(7);
	bar.b2 = unitBlackBox(4);
	assertPrint("int64_t bit fields support 3/N:", bar.a1);
	assertPrint("int64_t bit fields support 4/N:", bar.a2);
	assertPrint("int64_t bit fields support 5/N:", bar.b1);
	assertPrint("int64_t bit fields support 6/N:", bar.b2);


	// Test >32 bit field
	struct C {
		unsigned long long a : 34;
		unsigned long long b : 30;
	};
	struct C baz;
	baz.a = unitBlackBox(7);
	baz.b = unitBlackBox(4);
	// assertEqual(baz.a, 7ULL, "int64_t bit fields support 7/N");
	assertPrint("int64_t bit fields support 7/N:", baz.a);
	// assertEqual(baz.b, 4ULL, "int64_t bit fields support 8/N");
	assertPrint("int64_t bit fields support 8/N:", baz.b);


	// Test signed >32 bit field
	struct D {
		long long a : 34;
		long long b : 30;
	};
	struct D quux;
	quux.a = unitBlackBox(7);
	quux.b = unitBlackBox(-1);
	assertPrint("int64_t bit fields support 9/N:", 7);
	assertPrint("int64_t bit fields support 10/N:", -1);
	// assertEqual(quux.a, 7LL, "int64_t bit fields support 9/N");
	// assertEqual(quux.b, -1LL, "int64_t bit fields support 10/N");
}

template <typename T>
static void testSwitch()
{
	volatile T a = unitBlackBox<T>(0x00000001ff000000);
	bool result = false;
	switch(a)
	{
		case static_cast<T>(0x00000000ff000000):
			result = false;
			break;
		case static_cast<T>(0x00000001ff000000):
			result = true;
			break;
		case static_cast<T>(0x0000000100000000):
			result = false;
			break;
		default:
			result = false;
			break;
	}
	// assertEqual(result, true, "int64_t in switch statements");
	assertPrint("int64_t in switch statements:", result);
}

template <typename T>
static void testNew()
{
	volatile T a = unitBlackBox(10);
	// This should not crash while compiling
	int* buf = new int[a];
	delete[] buf;
}

template <typename T>
T testVaargImpl(int n, ...)
{
	T ret = 0;
	va_list vl;
	va_start(vl,n);
	for (int i=0;i<n;i++)
	{
		ret+=va_arg(vl,T);
	}
	va_end(vl);
	return ret;
}

template <typename T>
static void testVaarg()
{
	T sum = testVaargImpl<T>(unitBlackBox(2), unitBlackBox((T)0x100000000ll), unitBlackBox((T)0xdeadbeefll));
	assertPrint("int64_t vaarg support 1/1:", Hex(sum));
	// assertEqual(sum, (T)0x1deadbeef, "int64_t vaarg support 1/1");
}

#ifndef PRE_EXECUTE_TEST
static void testDump() {
	long long t = unitBlackBox(0xff000000ff000000);
	cheerp::console_log("test dump(0xff000000ff000000):");
	dump(t);
}
#endif

void webMain() {
	testRepresentation();
	//CHECK: int64_t representation 1/N: 0x0
	//CHECK: int64_t representation 2/N: -0x1
	//CHECK: int64_t representation 3/N: 0xffffffffffffffff
	//CHECK: int64_t representation 4/N: 0x7fffffffffffffff
#ifndef PRE_EXECUTE_TEST
	testDump();
	//CHECK-REG: test dump(0xff000000ff000000):
	//CHECK-REG: highint h: -0x1000000
	//CHECK-REG: highint l: -0x1000000
#endif
	testShiftOps<long long>();
	//CHECK: int64_t shl support 1/N: 0x10000
	//CHECK: int64_t shl support 2/N: 0x1000000
	//CHECK: int64_t shl support 3/N: 0x100000000
	//CHECK: int64_t shl support 4/N: 0x1000000000000
	//CHECK: int64_t shl support 5/N: 0x7fffffff

	//CHECK: int64_t shr support 1/N: 0x10000
	//CHECK: int64_t shr support 2/N: 0x100
	//CHECK: int64_t shr support 3/N: 0x10000
	//CHECK: int64_t shr support 4/N: 0x10000
	//CHECK: int64_t shr support 5/N: 0x7fffffff

	//CHECK-NOT: int64_t shr support 6/N: 0

	testShiftOps<unsigned long long>();
	//CHECK: int64_t shl support 1/N: 0x10000
	//CHECK: int64_t shl support 2/N: 0x1000000
	//CHECK: int64_t shl support 3/N: 0x100000000
	//CHECK: int64_t shl support 4/N: 0x1000000000000
	//CHECK: int64_t shl support 5/N: 0x7fffffff00000000

	//CHECK: int64_t shr support 1/N: 0x10000
	//CHECK: int64_t shr support 2/N: 0x100
	//CHECK: int64_t shr support 3/N: 0x10000
	//CHECK: int64_t shr support 4/N: 0x10000
	//CHECK: int64_t shr support 5/N: 0x7fffffff

	//CHECK-NOT: int64_t shr support 6/N: 0

	testBitwiseOps<long long>();
	//CHECK: int64_t AND support 1/N: 1
	//CHECK: int64_t AND support 2/N: 1

	//CHECK: int64_t OR support 1/N: 1
	//CHECK: int64_t OR support 2/N: 1
	//CHECK: int64_t OR support 3/N: 1
	//CHECK: int64_t OR support 4/N: 1

	//CHECK: int64_t XOR support 1/N: 1
	//CHECK: int64_t XOR support 2/N: 1
	//CHECK: int64_t XOR support 3/N: 1

	//CHECK: int64_t NOT support 1/N: 1
	//CHECK: int64_t NOT support 2/N: 1
	//CHECK: int64_t NOT support 3/N: 1

	testBitwiseOps<unsigned long long>();
	//CHECK: int64_t AND support 1/N: 1
	//CHECK: int64_t AND support 2/N: 1

	//CHECK: int64_t OR support 1/N: 1
	//CHECK: int64_t OR support 2/N: 1
	//CHECK: int64_t OR support 3/N: 1
	//CHECK: int64_t OR support 4/N: 1

	//CHECK: int64_t XOR support 1/N: 1
	//CHECK: int64_t XOR support 2/N: 1
	//CHECK: int64_t XOR support 3/N: 1

	//CHECK: int64_t NOT support 1/N: 1
	//CHECK: int64_t NOT support 2/N: 1
	//CHECK: int64_t NOT support 3/N: 1

	testAddSubOps<long long>();
	//CHECK: int64_t add support 1/N: 0x1deadbeaf
	//CHECK: int64_t add support 2/N: 0x100000000
	//CHECK: int64_t add support 3/N: 1
	//CHECK: int64_t add support 4/N: 1

	//CHECK: int64_t sub support 1/N: 0xdeadbeae
	//CHECK: int64_t sub support 2/N: 0xffffffff
	//CHECK: int64_t sub support 3/N: 0x0
	//CHECK: int64_t sub support 4/N: 1
	//CHECK: int64_t sub support 5/N: 0x0
	//CHECK: int64_t sub support 6/N: 1

	testAddSubOps<unsigned long long>();
	//CHECK: int64_t add support 1/N: 0x1deadbeaf
	//CHECK: int64_t add support 2/N: 0x100000000
	//CHECK: int64_t add support 3/N: 1
	//CHECK: int64_t add support 4/N: 1

	//CHECK: int64_t sub support 1/N: 0xdeadbeae
	//CHECK: int64_t sub support 2/N: 0xffffffff
	//CHECK: int64_t sub support 3/N: 0x0
	//CHECK: int64_t sub support 4/N: 1
	//CHECK: int64_t sub support 5/N: 0x0
	//CHECK: int64_t sub support 6/N: 1

	testMulDivModOps<long long>();
	//CHECK: int64_t mul support 1/N: 1
	//CHECK: int64_t mul support 2/N: 1
	//CHECK: int64_t mul support 3/N: 1
	//CHECK: int64_t mul support 4/N: 1
	//CHECK: int64_t mul support 5/N: 1

	//CHECK: int64_t div support 1/N: 1
	//CHECK: int64_t div support 2/N: 1
	//CHECK: int64_t div support 3/N: 1

	//CHECK: int64_t mod support 1/N: 1
	//CHECK: int64_t mod support 2/N: 1
	//CHECK: int64_t mod support 3/N: 1

	testMulDivModOps<unsigned long long>();
	//CHECK: int64_t mul support 1/N: 1
	//CHECK: int64_t mul support 2/N: 1
	//CHECK: int64_t mul support 3/N: 1
	//CHECK: int64_t mul support 4/N: 1
	//CHECK: int64_t mul support 5/N: 1

	//CHECK: int64_t div support 1/N: 1
	//CHECK: int64_t div support 2/N: 1
	//CHECK: int64_t div support 3/N: 1

	//CHECK: int64_t mod support 1/N: 1
	//CHECK: int64_t mod support 2/N: 1
	//CHECK: int64_t mod support 3/N: 1

	testUnaryOps<long long>();
	//CHECK: int64_t neg support 1/N: 1
	//CHECK: int64_t neg support 2/N: 1
	//CHECK: int64_t neg support 3/N: 1
	//CHECK: int64_t neg support 4/N: 1
	//CHECK: int64_t neg support 5/N: 1
	//CHECK: int64_t neg support 6/N: 1

	//CHECK: int64_t lnot support 1/N: 1
	//CHECK: int64_t lnot support 2/N: 0
	//CHECK: int64_t lnot support 3/N: 0
	//CHECK: int64_t lnot support 4/N: 0

	testUnaryOps<unsigned long long>();
	//CHECK: int64_t neg support 1/N: 1
	//CHECK: int64_t neg support 2/N: 1
	//CHECK: int64_t neg support 3/N: 1
	//CHECK: int64_t neg support 4/N: 1
	//CHECK: int64_t neg support 5/N: 1
	//CHECK: int64_t neg support 6/N: 1

	//CHECK: int64_t lnot support 1/N: 1
	//CHECK: int64_t lnot support 2/N: 0
	//CHECK: int64_t lnot support 3/N: 0
	//CHECK: int64_t lnot support 4/N: 0

	testBitwiseCompoundAssignmentOps<long long>();
	//CHECK: int64_t and assign support 1/N: 0x0
	//CHECK: int64_t and assign support 2/N: 0x0
	//CHECK: int64_t and assign support 3/N: 0x100000000
	//CHECK: int64_t and assign support 4/N: 0x0

	//CHECK: int64_t or assign support 1/N: 0x1
	//CHECK: int64_t or assign support 2/N: 0x100000001
	//CHECK: int64_t or assign support 3/N: 1
	//CHECK: int64_t or assign support 4/N: 1

	//CHECK: int64_t xor assign support 1/N: 0x1
	//CHECK: int64_t xor assign support 2/N: 0x100000001
	//CHECK: int64_t xor assign support 3/N: 1
	//CHECK: int64_t xor assign support 4/N: 1

	testBitwiseCompoundAssignmentOps<unsigned long long>();
	//CHECK: int64_t and assign support 1/N: 0x0
	//CHECK: int64_t and assign support 2/N: 0x0
	//CHECK: int64_t and assign support 3/N: 0x100000000
	//CHECK: int64_t and assign support 4/N: 0x0

	//CHECK: int64_t or assign support 1/N: 0x1
	//CHECK: int64_t or assign support 2/N: 0x100000001
	//CHECK: int64_t or assign support 3/N: 1
	//CHECK: int64_t or assign support 4/N: 1

	//CHECK: int64_t xor assign support 1/N: 0x1
	//CHECK: int64_t xor assign support 2/N: 0x100000001
	//CHECK: int64_t xor assign support 3/N: 1
	//CHECK: int64_t xor assign support 4/N: 1

	testArithmeticCompoundAssignmentOps<long long>();
	//CHECK: int64_t add assign support 1/N: 0x1
	//CHECK: int64_t add assign support 2/N: 0x100000001
	//CHECK: int64_t add assign support 3/N: 0xffffffff
	//CHECK: int64_t add assign support 4/N: 1

	//CHECK: int64_t sub assign support 1/N: 1
	//CHECK: int64_t sub assign support 2/N: 1
	//CHECK: int64_t sub assign support 3/N: 1
	//CHECK: int64_t sub assign support 4/N: 1

	//CHECK: int64_t mod assign support 1/N: 0x0
	//CHECK: int64_t mod assign support 2/N: 0x1
	//CHECK: int64_t mod assign support 3/N: 1 
	//CHECK: int64_t mod assign support 4/N: 0x0

	//CHECK: int64_t mul assign support 1/N: 0x0
	//CHECK: int64_t mul assign support 2/N: 0x100000000
	//CHECK: int64_t mul assign support 3/N: 1
	//CHECK: int64_t mul assign support 4/N: 1

	//CHECK: int64_t div assign support 1/N: 0x0
	//CHECK: int64_t div assign support 2/N: 0x0
	//CHECK: int64_t mod assign support 3/N: 1

	//CHECK: int64_t shl assign support 1/N: 0x1
	//CHECK: int64_t shl assign support 2/N: 1
	//CHECK: int64_t shl assign support 3/N: 1

	//CHECK: int64_t shr assign support 1/N: 0x1
	//CHECK: int64_t shr assign support 2/N: 0x10000
	//CHECK: int64_t shr assign support 3/N: 1

	//CHECK: int64_t compound assign result support: 1

	testArithmeticCompoundAssignmentOps<unsigned long long>();
	//CHECK: int64_t add assign support 1/N: 0x1
	//CHECK: int64_t add assign support 2/N: 0x100000001
	//CHECK: int64_t add assign support 3/N: 0xffffffff
	//CHECK: int64_t add assign support 4/N: 1

	//CHECK: int64_t sub assign support 1/N: 1
	//CHECK: int64_t sub assign support 2/N: 1
	//CHECK: int64_t sub assign support 3/N: 1
	//CHECK: int64_t sub assign support 4/N: 1

	//CHECK: int64_t mod assign support 1/N: 0x0
	//CHECK: int64_t mod assign support 2/N: 0x1
	//CHECK: int64_t mod assign support 3/N: 1 
	//CHECK: int64_t mod assign support 4/N: 0x0

	//CHECK: int64_t mul assign support 1/N: 0x0
	//CHECK: int64_t mul assign support 2/N: 0x100000000
	//CHECK: int64_t mul assign support 3/N: 1
	//CHECK: int64_t mul assign support 4/N: 1

	//CHECK: int64_t div assign support 1/N: 0x0
	//CHECK: int64_t div assign support 2/N: 0x0
	//CHECK: int64_t mod assign support 3/N: 1

	//CHECK: int64_t shl assign support 1/N: 0x1
	//CHECK: int64_t shl assign support 2/N: 1
	//CHECK: int64_t shl assign support 3/N: 1

	//CHECK: int64_t shr assign support 1/N: 0x1
	//CHECK: int64_t shr assign support 2/N: 0x10000
	//CHECK: int64_t shr assign support 3/N: 1

	//CHECK: int64_t compound assign result support: 1

	testComparisonOps<long long>();
	//CHECK: int64_t lt support 1/N: 1
	//CHECK: int64_t lt support 2/N: 1
	//CHECK: int64_t lt support 3/N: 1
	//CHECK: int64_t lt support 4/N: 1
	//CHECK: int64_t lt support 5/N: 1
	//CHECK: int64_t lt support 6/N: 0
	//CHECK: int64_t lt support 7/N: 0
	//CHECK: int64_t lt support 8/N: 1

	//CHECK: int64_t le support 1/N: 1
	//CHECK: int64_t le support 2/N: 1
	//CHECK: int64_t le support 3/N: 1
	//CHECK: int64_t le support 4/N: 1
	//CHECK: int64_t le support 5/N: 1
	//CHECK: int64_t le support 6/N: 0
	//CHECK: int64_t le support 7/N: 1
	//CHECK: int64_t le support 8/N: 1

	//CHECK: int64_t gt support 1/N: 0
	//CHECK: int64_t gt support 2/N: 0
	//CHECK: int64_t gt support 3/N: 1
	//CHECK: int64_t gt support 4/N: 1
	//CHECK: int64_t gt support 5/N: 0
	//CHECK: int64_t gt support 6/N: 1
	//CHECK: int64_t gt support 7/N: 0 
	//CHECK: int64_t gt support 8/N: 1

	//CHECK: int64_t ge support 1/N: 0
	//CHECK: int64_t ge support 2/N: 0
	//CHECK: int64_t ge support 3/N: 1
	//CHECK: int64_t ge support 4/N: 1
	//CHECK: int64_t ge support 5/N: 0
	//CHECK: int64_t ge support 6/N: 1
	//CHECK: int64_t ge support 7/N: 1 
	//CHECK: int64_t ge support 8/N: 1

	//CHECK: int64_t zero is false 1/N: 1

	testComparisonOps<unsigned long long>();
	//CHECK: int64_t lt support 1/N: 1
	//CHECK: int64_t lt support 2/N: 1
	//CHECK: int64_t lt support 3/N: 1
	//CHECK: int64_t lt support 4/N: 1
	//CHECK: int64_t lt support 5/N: 1
	//CHECK: int64_t lt support 6/N: 0
	//CHECK: int64_t lt support 7/N: 0
	//CHECK: int64_t lt support 8/N: 1

	//CHECK: int64_t le support 1/N: 1
	//CHECK: int64_t le support 2/N: 1
	//CHECK: int64_t le support 3/N: 1
	//CHECK: int64_t le support 4/N: 1
	//CHECK: int64_t le support 5/N: 1
	//CHECK: int64_t le support 6/N: 0
	//CHECK: int64_t le support 7/N: 1
	//CHECK: int64_t le support 8/N: 1

	//CHECK: int64_t gt support 1/N: 0
	//CHECK: int64_t gt support 2/N: 0
	//CHECK: int64_t gt support 3/N: 1
	//CHECK: int64_t gt support 4/N: 1
	//CHECK: int64_t gt support 5/N: 0
	//CHECK: int64_t gt support 6/N: 1
	//CHECK: int64_t gt support 7/N: 0 
	//CHECK: int64_t gt support 8/N: 1

	//CHECK: int64_t ge support 1/N: 0
	//CHECK: int64_t ge support 2/N: 0
	//CHECK: int64_t ge support 3/N: 1
	//CHECK: int64_t ge support 4/N: 1
	//CHECK: int64_t ge support 5/N: 0
	//CHECK: int64_t ge support 6/N: 1
	//CHECK: int64_t ge support 7/N: 1 
	//CHECK: int64_t ge support 8/N: 1

	//CHECK: int64_t zero is false 1/N: 1
	
	testCastToFloat<long long>();
	//CHECK: int64_t cast to float support 1/N: 1
	//CHECK: int64_t cast to float support 2/N: 1
	//CHECK: int64_t cast to float support 3/N: 1
	//CHECK: int64_t cast to float support 4/N: 1
	//CHECK: int64_t cast to float support 6/N: 1
	//CHECK: int64_t cast to float support 7/N: 1
	//CHECK: int64_t cast to float support 8/N: 1

	testCastToFloat<unsigned long long>();

	//CHECK: int64_t cast to float support 1/N: 1
	//CHECK: int64_t cast to float support 2/N: 1
	//CHECK: int64_t cast to float support 3/N: 1
	//CHECK: int64_t cast to float support 4/N: 1
	//CHECK: int64_t cast to float support 5/N: 1

	testCastFromFloat<long long>();
	//CHECK: int64_t cast from float support 1/N: 0
	//CHECK: int64_t cast from float support 2/N: 1
	//CHECK: int64_t cast from float support 3/N: 1337
	//CHECK: int64_t cast from float support 4/N: 3
	//CHECK: int64_t cast from float support 5/N: -1
	//CHECK: int64_t cast from float support 6/N: -3200
	//CHECK: int64_t cast from float support 7/N: -3200000000
	//CHECK: int64_t cast from float support 8/N: 9223372036854771712

	testCastFromFloat<unsigned long long>();
	//CHECK: int64_t cast from float support 1/N: 0
	//CHECK: int64_t cast from float support 2/N: 1
	//CHECK: int64_t cast from float support 3/N: 1337
	//CHECK: int64_t cast from float support 4/N: 3

	testIncrement<long long>();
	//CHECK: int64_t post increment support 1/3: 1
	//CHECK: int64_t post increment support 2/3: 0xffffffff
	//CHECK: int64_t post increment support 3/3: 0x100000000

	//CHECK: int64_t pre increment support 1/3: 0x1
	//CHECK: int64_t pre increment support 2/3: 0x100000000
	//CHECK: int64_t pre increment support 3/3: 0x100000000

	//CHECK: int64_t post decrement support 1/3: -0x1
	//CHECK: int64_t post decrement support 2/3: -0x1
	//CHECK: int64_t post decrement support 3/3: -0x2

	//CHECK: int64_t pre decrement support 1/3: -0x1
	//CHECK: int64_t pre decrement support 2/3: -0x2
	//CHECK: int64_t pre decrement support 3/3: -0x2

	testIncrement<unsigned long long>();
	//CHECK: int64_t post increment support 1/3: 1
	//CHECK: int64_t post increment support 2/3: 0xffffffff
	//CHECK: int64_t post increment support 3/3: 0x100000000

	//CHECK: int64_t pre increment support 1/3: 0x1
	//CHECK: int64_t pre increment support 2/3: 0x100000000
	//CHECK: int64_t pre increment support 3/3: 0x100000000

	//CHECK: int64_t post decrement support 1/3: 0xffffffffffffffff
	//CHECK: int64_t post decrement support 2/3: 0xffffffffffffffff
	//CHECK: int64_t post decrement support 3/3: 0xfffffffffffffffe

	//CHECK: int64_t pre decrement support 1/3: 0xffffffffffffffff
	//CHECK: int64_t pre decrement support 2/3: 0xfffffffffffffffe
	//CHECK: int64_t pre decrement support 3/3: 0xfffffffffffffffe

	testStructs<long long>();
	//CHECK: int64_t struct support 1/3: 1
	//CHECK: int64_t struct support 2/3: 1
	//CHECK: int64_t struct support 3/3: 1

	testStructs<unsigned long long>();
	//CHECK: int64_t struct support 1/3: 1
	//CHECK: int64_t struct support 2/3: 1
	//CHECK: int64_t struct support 3/3: 1

	testPointerArithmetic<long long>();
	//CHECK: int64_t list support 1/2: 1
	//CHECK: int64_t list support 2/2: 1 

	testPointerArithmetic<unsigned long long>();
	//CHECK: int64_t list support 1/2: 1
	//CHECK: int64_t list support 2/2: 1 

	testPointersAndReferences<long long>();
	//CHECK: int64_t reference support 1/N: 43
	
	testPointersAndReferences<unsigned long long>();
	//CHECK: int64_t reference support 1/N: 43

	testBitfields();
	//CHECK: int64_t bit fields support 1/N: 7
	//CHECK: int64_t bit fields support 2/N: 4
	//CHECK: int64_t bit fields support 3/N: 7
	//CHECK: int64_t bit fields support 4/N: 4
	//CHECK: int64_t bit fields support 5/N: 7
	//CHECK: int64_t bit fields support 6/N: 4
	//CHECK: int64_t bit fields support 7/N: 7
	//CHECK: int64_t bit fields support 8/N: 4
	//CHECK: int64_t bit fields support 9/N: 7
	//CHECK: int64_t bit fields support 10/N: -1


	testNew<long long>();
	// this test does not return anything, it should not crash

	testNew<unsigned long long>();
	// this test does not return anything, it should not crash
	
	testVaarg<long long>();
	//CHECK: int64_t vaarg support 1/1: 0x1deadbeef

	testVaarg<unsigned long long>();
	//CHECK: int64_t vaarg support 1/1: 0x1deadbeef

	testSwitch<long long>();
	//CHECK: int64_t in switch statements: 1

	testSwitch<unsigned long long>();
	//CHECK: int64_t in switch statements: 1
}

// vim: noexpandtab

