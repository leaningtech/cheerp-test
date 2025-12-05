//===---------------------------------------------------------------------===//
//	Copyright 2013 Leaning Technlogies
//===----------------------------------------------------------------------===//

#ifndef _CHEERP_TESTS_H
#define _CHEERP_TESTS_H
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <cheerp/types.h>
#include <cheerp/client.h>

extern "C" void agnosticPrintString(const char* msg)
#ifdef PRE_EXECUTE_TEST
;
#else
{
	cheerp::console_log(msg);}
#endif

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
	else if constexpr (std::is_same_v<T, const char *>){
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

template<typename... Args>
[[cheerp::genericjs]]
void __preexecute_print_case(const char *msg, Args... vals)
#ifdef PRE_EXECUTE_TEST
; // defined in preexecuter
#else
{
	client::String *buffer = new client::String(msg);

	((buffer = format_value(buffer, vals)), ...);

	cheerp::console_log(buffer);
}
#endif

void assertEqual(double value, double expected, double epsilon, const char* msg)
{
	if (value >= expected - epsilon && value <= expected + epsilon){
		__preexecute_print_case(msg, value);
	}
	else{
		agnosticPrintString("FAILURE");
	}
}

void assertEqual(const char *value, const char *expected, const char* msg)
{
	if (strcmp(value, expected) == 0)
		__preexecute_print_case(msg, value);
	else{
		agnosticPrintString("FAILURE");
	}
}

template<class T>
void assertEqual(const T value, const T expected, const char* msg)
{
	if (value == expected){
		__preexecute_print_case(msg, value);
	}
	else{
		agnosticPrintString("FAILURE");
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
