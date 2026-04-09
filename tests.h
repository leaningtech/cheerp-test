//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//

#ifndef _CHEERP_TESTS_H
#define _CHEERP_TESTS_H

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <type_traits>
#include <cctype>
#include <cheerp/types.h>
#include <cheerp/client.h>

namespace test_format {
	static inline size_t str_copy(char* dest, const char* src, size_t max) {
		size_t i = 0;
		while (src[i] && i < max - 1) {
			dest[i] = src[i];
			i++;
		}
		return i;
	}

	static inline size_t uint_to_hex(char* buf, unsigned long long val) {
		if (val == 0) {
			buf[0] = '0';
			return 1;
		}
		char temp[20];
		size_t i = 0;
		while (val > 0) {
			int digit = val & 0xF;
			temp[i++] = digit < 10 ? '0' + digit : 'a' + (digit - 10);
			val >>= 4;
		}
		for (size_t j = 0; j < i; j++) {
			buf[j] = temp[i - 1 - j];
		}
		return i;
	}

	static inline size_t uint_to_dec(char* buf, unsigned long long val) {
		if (val == 0) {
			buf[0] = '0';
			return 1;
		}
		char temp[20];
		size_t i = 0;
		while (val > 0) {
			temp[i++] = '0' + (val % 10);
			val /= 10;
		}
		for (size_t j = 0; j < i; j++) {
			buf[j] = temp[i - 1 - j];
		}
		return i;
	}

	static inline size_t int_to_dec(char* buf, long long val) {
		if (val < 0) {
			buf[0] = '-';
			return 1 + uint_to_dec(buf + 1, (unsigned long long)(-val));
		}
		return uint_to_dec(buf, (unsigned long long)val);
	}
}

// Wrapper for printing values in hexadecimal format
template<typename T>
struct Hex {
	T value;
	explicit Hex(T v) : value(v) {}
};

// In PRE_EXECUTE_TEST mode, __preexecute_print_case is defined by the

// PREEXECUTE MODE ----------------------------------------------------------
#ifdef PRE_EXECUTE_TEST

// General case is provided by preexecuter
template<typename... Args>
extern void __preexecute_print_case(const char *msg, Args... vals);

// Special handling for Hex in preexecution mode - format it ourselves
// to keep the same format across modes.
template<typename T>
void __preexecute_print_case(const char *msg, Hex<T> hex) {
	char buffer[256];
	size_t offset = 0;
	if constexpr (std::is_integral_v<T>) {
		// Copy message
		offset = test_format::str_copy(buffer, msg, sizeof(buffer));
		if (offset < sizeof(buffer) - 1) buffer[offset++] = ' ';
		
		// Handle signed negative values
		if constexpr (std::is_signed_v<T>) {
			if (hex.value < 0) {
				if (offset < sizeof(buffer) - 1) buffer[offset++] = '-';
				if (offset < sizeof(buffer) - 2) {
					buffer[offset++] = '0';
					buffer[offset++] = 'x';
				}
				if constexpr (sizeof(T) <= sizeof(int)) {
					offset += test_format::uint_to_hex(buffer + offset, (unsigned int)(-hex.value));
				} else {
					offset += test_format::uint_to_hex(buffer + offset, (unsigned long long)(-hex.value));
				}
			} else {
				if (offset < sizeof(buffer) - 2) {
					buffer[offset++] = '0';
					buffer[offset++] = 'x';
				}
				if constexpr (sizeof(T) <= sizeof(int)) {
					offset += test_format::uint_to_hex(buffer + offset, (unsigned int)hex.value);
				} else {
					offset += test_format::uint_to_hex(buffer + offset, (unsigned long long)hex.value);
				}
			}
		} else {
			if (offset < sizeof(buffer) - 2) {
				buffer[offset++] = '0';
				buffer[offset++] = 'x';
			}
			if constexpr (sizeof(T) <= sizeof(int)) {
				offset += test_format::uint_to_hex(buffer + offset, (unsigned int)hex.value);
			} else {
				offset += test_format::uint_to_hex(buffer + offset, (unsigned long long)hex.value);
			}
		}
	} else {
		offset = test_format::str_copy(buffer, msg, sizeof(buffer));
	}
	buffer[offset] = '\0';
	__preexecute_print_case(buffer);
}

// Special handling for char types in preexecution mode - format as string
inline void __preexecute_print_case(const char *msg, char value) {
	char buffer[256];
	size_t offset = test_format::str_copy(buffer, msg, sizeof(buffer));
	if (offset < sizeof(buffer) - 1) buffer[offset++] = ' ';
	if (offset < sizeof(buffer) - 1) buffer[offset++] = value;
	buffer[offset] = '\0';
	__preexecute_print_case(buffer);
}

inline void __preexecute_print_case(const char *msg, signed char value) {
	char buffer[256];
	size_t offset = test_format::str_copy(buffer, msg, sizeof(buffer));
	if (offset < sizeof(buffer) - 1) buffer[offset++] = ' ';
	if (offset < sizeof(buffer) - 1) buffer[offset++] = value;
	buffer[offset] = '\0';
	__preexecute_print_case(buffer);
}

inline void __preexecute_print_case(const char *msg, unsigned char value) {
	char buffer[256];
	size_t offset = test_format::str_copy(buffer, msg, sizeof(buffer));
	if (offset < sizeof(buffer) - 1) buffer[offset++] = ' ';
	if (offset < sizeof(buffer) - 1) buffer[offset++] = value;
	buffer[offset] = '\0';
	__preexecute_print_case(buffer);
}

// Wrapper with more descriptive name for printing multiple values
template<typename T, typename... Args>
void print_multi_vals(const char *msg, T first, Args... rest){
	__preexecute_print_case(msg, first, rest...);
}

#else // !PRE_EXECUTE_TEST

// REGULAR MODE ------------------------------------------------------------
// Here we implement __preexecute_print_case in terms of cheerp::console_log.

// Helper to format a single value into a buffer
template<typename T>
void formatSingleValue(char* buffer, size_t bufsize, size_t& offset, T value) {
	if (offset >= bufsize - 1) return;
	if constexpr (std::is_same_v<T, double> || std::is_same_v<T, float>) {
    // snprintf is still used heren, should ideally be replaced
    offset += snprintf(buffer + offset, bufsize - offset, " %.10f", (double)value);
	} else if constexpr (std::is_same_v<T, const char*> || std::is_same_v<T, char*>) {
		if (offset < bufsize - 1) buffer[offset++] = ' ';
		if (value) {
			offset += test_format::str_copy(buffer + offset, value, bufsize - offset);
		} else {
			offset += test_format::str_copy(buffer + offset, "nullptr", bufsize - offset);
		}
	} else if constexpr (std::is_same_v<T, char> || std::is_same_v<T, signed char> || std::is_same_v<T, unsigned char>) {
		if (offset < bufsize - 1) buffer[offset++] = ' ';
		if (offset < bufsize - 1) buffer[offset++] = value;
	} else if constexpr (std::is_integral_v<T>) {
		if (offset < bufsize - 1) buffer[offset++] = ' ';
		if constexpr (std::is_unsigned_v<T>) {
			if constexpr (sizeof(T) <= sizeof(unsigned int)) {
				offset += test_format::uint_to_dec(buffer + offset, (unsigned int)value);
			} else {
				offset += test_format::uint_to_dec(buffer + offset, (unsigned long long)value);
			}
		} else {
			if constexpr (sizeof(T) <= sizeof(int)) {
				offset += test_format::int_to_dec(buffer + offset, (int)value);
			} else {
				offset += test_format::int_to_dec(buffer + offset, (long long)value);
			}
		}
	}
}

// Overload for Hex wrapper
template<typename T>
void formatSingleValue(char* buffer, size_t bufsize, size_t& offset, Hex<T> hex) {
	if (offset >= bufsize - 1) return;
	if constexpr (std::is_integral_v<T>) {
		if (offset < bufsize - 1) buffer[offset++] = ' ';
		if constexpr (std::is_signed_v<T>) {
			if (hex.value < 0) {
				if (offset < bufsize - 1) buffer[offset++] = '-';
				if (offset < bufsize - 2) {
					buffer[offset++] = '0';
					buffer[offset++] = 'x';
				}
				if constexpr (sizeof(T) <= sizeof(int)) {
					offset += test_format::uint_to_hex(buffer + offset, (unsigned int)(-hex.value));
				} else {
					offset += test_format::uint_to_hex(buffer + offset, (unsigned long long)(-hex.value));
				}
			} else {
				if (offset < bufsize - 2) {
					buffer[offset++] = '0';
					buffer[offset++] = 'x';
				}
				if constexpr (sizeof(T) <= sizeof(int)) {
					offset += test_format::uint_to_hex(buffer + offset, (unsigned int)hex.value);
				} else {
					offset += test_format::uint_to_hex(buffer + offset, (unsigned long long)hex.value);
				}
			}
		} else {
			if (offset < bufsize - 2) {
				buffer[offset++] = '0';
				buffer[offset++] = 'x';
			}
			if constexpr (sizeof(T) <= sizeof(int)) {
				offset += test_format::uint_to_hex(buffer + offset, (unsigned int)hex.value);
			} else {
				offset += test_format::uint_to_hex(buffer + offset, (unsigned long long)hex.value);
			}
		}
	}
}

// Multiple values version (variadic)
template<typename... Args>
void __preexecute_print_case(const char *msg, Args... vals) {
	char buffer[512];
	size_t offset = test_format::str_copy(buffer, msg, sizeof(buffer));
	(formatSingleValue(buffer, sizeof(buffer), offset, vals), ...);
	buffer[offset] = '\0';
	cheerp::console_log(buffer);
}

// Wrapper with more descriptive name for printing multiple values
template<typename T, typename... Args>
void print_multi_vals(const char *msg, T first, Args... rest){
	__preexecute_print_case(msg, first, rest...);
}

#endif // PRE_EXECUTE_TEST

#ifdef __CHEERP__
template<typename T>
[[cheerp::genericjs]]
client::String *format_value(client::String *base, T value){
	base = base->concat(" ");

	if constexpr (std::is_same_v<T, double>){
		client::Number* num = new client::Number(static_cast<double>(value));
        return base->concat(num->toFixed(10));
	}
	else if constexpr(std::is_same_v<T, float>){
		client::Number* num = new client::Number(static_cast<double>(value));
        return base->concat(num->toFixed(10));
	}
	else if constexpr (std::is_same_v<T, const char *> || std::is_same_v<T, char *>){
		if (value == nullptr){
			return base->concat("nullptr");
		}
		client::String * str = new client::String(value);
		return base->concat(str);
	}
	else if constexpr (std::is_same_v<T, client::String*>){
		return base->concat(value);
	}
	else if constexpr(std::is_integral_v<T>){
		client::Number* num = new client::Number(static_cast<double>(value));
        return base->concat(num->toString());
	}
	else{
		return base;
	}
}

// Overload for client::String* - only available in genericjs
[[cheerp::genericjs]]
void assertPrint(const char* msg, client::String* value)
{
	client::String *buffer = new client::String(msg);
	buffer = format_value(buffer, value);
	cheerp::console_log(buffer);
}
#endif // __CHEERP__

// Universal assertPrint functions – thin wrappers around __preexecute_print_case
// function name should be improved, no assertion actually happens
inline void assertPrint(const char* msg)
{
	__preexecute_print_case(msg);
}

inline void assertPrint(const char* msg, double value)
{
	__preexecute_print_case(msg, value);
}

inline void assertPrint(const char* msg, char * value)
{
	__preexecute_print_case(msg, value);
}

inline void assertPrint(const char* msg, const char * value)
{
	__preexecute_print_case(msg, value);
}

template<typename T>
inline void assertPrint(const char* msg, T value)
{
	__preexecute_print_case(msg, value);
}

// Overload for Hex wrapper
template<typename T>
inline void assertPrint(const char* msg, Hex<T> hex)
{
	__preexecute_print_case(msg, hex);
}

void assertEqual(double value, double expected, double epsilon, const char* msg)
{
	if (value >= expected - epsilon && value <= expected + epsilon){
		__preexecute_print_case(msg, value);
	}
	else{
		__preexecute_print_case("FAILURE");
	}
}

void assertEqual(const char *value, const char *expected, const char* msg)
{
	if (strcmp(value, expected) == 0)
		__preexecute_print_case(msg, value);
	else{
		__preexecute_print_case("FAILURE");
	}
}

void assertUnequal(const char *value, const char *expected, const char* msg)
{
	if (strcmp(value, expected) != 0)
		__preexecute_print_case(msg, value);
	else{
		__preexecute_print_case("FAILURE");
	}
}

template<class T>
void assertEqual(const T &value, const T &expected, const char* msg)
{
	if (!(value == expected)){
		__preexecute_print_case("FAILURE");
		return;
	}
	if constexpr (std::is_pointer_v<T>) {
		if (expected == nullptr)
			__preexecute_print_case(msg, "SUCCESS");
		else
			__preexecute_print_case(msg, value);
	}
	else if constexpr (std::is_integral_v<T> || std::is_floating_point_v<T> || std::is_enum_v<T>) {
		if (expected == static_cast<T>(0))
			__preexecute_print_case(msg, "SUCCESS");
		else
			__preexecute_print_case(msg, value);
	}
	else {
		__preexecute_print_case(msg, value);
	}
}

template <typename T>	
[[clang::optnone]]
T unitBlackBox(T v)
{
	static T a[1];
	volatile T* volatile p = &a[0];
	a[0] = v;
	v = *p;
	return v;
}

#endif
