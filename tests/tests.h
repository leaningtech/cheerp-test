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

// Wrapper for printing values in hexadecimal format
template<typename T>
struct Hex {
	T value;
	explicit Hex(T v) : value(v) {}
};

// In PRE_EXECUTE_TEST mode, __preexecute_print_case is defined by the
// preexecuter (PreExecute.cpp). In regular mode we implement it here using
// printf-style formatting and cheerp::console_log.

// PREEXECUTE MODE ----------------------------------------------------------
#ifdef PRE_EXECUTE_TEST

// General case is provided by preexecuter
template<typename... Args>
extern void __preexecute_print_case(const char *msg, Args... vals);

// Special handling for Hex in preexecution mode - format it ourselves
// to keep the same textual format across modes.
template<typename T>
void __preexecute_print_case(const char *msg, Hex<T> hex) {
	char buffer[256];
	if constexpr (std::is_integral_v<T>) {
		// Handle signed negative values
		if constexpr (std::is_signed_v<T>) {
			if (hex.value < 0) {
				if constexpr (sizeof(T) <= sizeof(int)) {
					snprintf(buffer, sizeof(buffer), "%s -0x%x", msg, (unsigned int)(-hex.value));
				} else {
					snprintf(buffer, sizeof(buffer), "%s -0x%llx", msg, (unsigned long long)(-hex.value));
				}
			} else {
				if constexpr (sizeof(T) <= sizeof(int)) {
					snprintf(buffer, sizeof(buffer), "%s 0x%x", msg, (unsigned int)hex.value);
				} else {
					snprintf(buffer, sizeof(buffer), "%s 0x%llx", msg, (unsigned long long)hex.value);
				}
			}
		} else {
			if constexpr (sizeof(T) <= sizeof(int)) {
				snprintf(buffer, sizeof(buffer), "%s 0x%x", msg, (unsigned int)hex.value);
			} else {
				snprintf(buffer, sizeof(buffer), "%s 0x%llx", msg, (unsigned long long)hex.value);
			}
		}
	} else {
		snprintf(buffer, sizeof(buffer), "%s", msg);
	}
	__preexecute_print_case(buffer);
}

// Special handling for char types in preexecution mode - format as string
inline void __preexecute_print_case(const char *msg, char value) {
	char buffer[256];
	snprintf(buffer, sizeof(buffer), "%s %c", msg, value);
	__preexecute_print_case(buffer);
}

inline void __preexecute_print_case(const char *msg, signed char value) {
	char buffer[256];
	snprintf(buffer, sizeof(buffer), "%s %c", msg, value);
	__preexecute_print_case(buffer);
}

inline void __preexecute_print_case(const char *msg, unsigned char value) {
	char buffer[256];
	snprintf(buffer, sizeof(buffer), "%s %c", msg, value);
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
		offset += snprintf(buffer + offset, bufsize - offset, " %.10f", (double)value);
	} else if constexpr (std::is_same_v<T, const char*> || std::is_same_v<T, char*>) {
		offset += snprintf(buffer + offset, bufsize - offset, " %s", value ? value : "nullptr");
	} else if constexpr (std::is_same_v<T, char> || std::is_same_v<T, signed char> || std::is_same_v<T, unsigned char>) {
		offset += snprintf(buffer + offset, bufsize - offset, " %c", value);
	} else if constexpr (std::is_integral_v<T>) {
		if constexpr (std::is_unsigned_v<T>) {
			if constexpr (sizeof(T) <= sizeof(unsigned int)) {
				offset += snprintf(buffer + offset, bufsize - offset, " %u", (unsigned int)value);
			} else {
				offset += snprintf(buffer + offset, bufsize - offset, " %llu", (unsigned long long)value);
			}
		} else {
			if constexpr (sizeof(T) <= sizeof(int)) {
				offset += snprintf(buffer + offset, bufsize - offset, " %d", (int)value);
			} else {
				offset += snprintf(buffer + offset, bufsize - offset, " %lld", (long long)value);
			}
		}
	}
}

// Overload for Hex wrapper
template<typename T>
void formatSingleValue(char* buffer, size_t bufsize, size_t& offset, Hex<T> hex) {
	if (offset >= bufsize - 1) return;
	if constexpr (std::is_integral_v<T>) {
		if constexpr (std::is_signed_v<T>) {
			if (hex.value < 0) {
				if constexpr (sizeof(T) <= sizeof(int)) {
					offset += snprintf(buffer + offset, bufsize - offset, " -0x%x", (unsigned int)(-hex.value));
				} else {
					offset += snprintf(buffer + offset, bufsize - offset, " -0x%llx", (unsigned long long)(-hex.value));
				}
			} else {
				if constexpr (sizeof(T) <= sizeof(int)) {
					offset += snprintf(buffer + offset, bufsize - offset, " 0x%x", (unsigned int)hex.value);
				} else {
					offset += snprintf(buffer + offset, bufsize - offset, " 0x%llx", (unsigned long long)hex.value);
				}
			}
		} else {
			if constexpr (sizeof(T) <= sizeof(int)) {
				offset += snprintf(buffer + offset, bufsize - offset, " 0x%x", (unsigned int)hex.value);
			} else {
				offset += snprintf(buffer + offset, bufsize - offset, " 0x%llx", (unsigned long long)hex.value);
			}
		}
	}
}

// Multiple values version (variadic)
template<typename... Args>
void __preexecute_print_case(const char *msg, Args... vals) {
	char buffer[512];
	size_t offset = snprintf(buffer, sizeof(buffer), "%s", msg);
	(formatSingleValue(buffer, sizeof(buffer), offset, vals), ...);
	cheerp::console_log(buffer);
}

// Wrapper with more descriptive name for printing multiple values
template<typename T, typename... Args>
void print_multi_vals(const char *msg, T first, Args... rest){
	__preexecute_print_case(msg, first, rest...);
}

#endif // PRE_EXECUTE_TEST

// GenericJS-only functions for client::String support
#ifdef __CHEERP_CLIENT__
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
#endif // __CHEERP_CLIENT__

// Universal assertPrint functions – thin wrappers around __preexecute_print_case
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

	// Preserve the historical behavior:
	// - pointers: when equal and null -> print "SUCCESS", otherwise print the value
	// - integral/enum/fp: when equal and zero -> print "SUCCESS", otherwise print the value
	// - other types (e.g. std::string): print the message (and value if supported)
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
		// For non-scalar types we don't have a stable, portable printing format.
		// In regular mode the value will typically be ignored by the formatter.
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
