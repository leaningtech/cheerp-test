//===---------------------------------------------------------------------===//
//	Copyright 2025 Leaning Technologies
//===----------------------------------------------------------------------===//
// Tries to construct `Task::promise_type` using the generic conversion

// REQUIRES: js, regular

// RUN: mkdir -p %t
// RUN: regular_only run_if_js compile_mode_js -std=c++20 -o %t/j %s 2>&1 | %FileCheck %s

#include <coroutine>

#include <cheerp/jsobject.h>

struct Task {
	struct promise_type {
		// Ensure `promise_type` does not fit inside a register, so that we get an
		// error if the conversion described below is selected.
		char _[1024];

		Task get_return_object();
		std::suspend_never initial_suspend() const noexcept;
		std::suspend_never final_suspend() const noexcept;
		void return_void();
	};
};

// Tries to construct `Task::promise_type` using the generic conversion
// `operator T()` on `client::_Any`. The conversion should not be considered by
// overload resolution because of SFINAE.
Task foo(const client::Object&) {
	co_return;
}

// CHECK-NOT: error