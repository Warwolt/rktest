/*
------------------------------------------------------------------------------
Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------
*/

#include <stdbool.h>
#include <stddef.h>

#ifndef RKTEST_H
#define RKTEST_H

// RKTest is a small unit test library for C99 with an interface heavily based on Google Test.

/* Config variables --------------------------------------------------------- */
// Define these variables before including rktest.h to configure the max number
// of supported test suites and max number of unit tests per suite.

#ifndef RKTEST_MAX_NUM_TEST_SUITES
#define RKTEST_MAX_NUM_TEST_SUITES 512
#endif

#ifndef RKTEST_MAX_NUM_TESTS_PER_SUITE
#define RKTEST_MAX_NUM_TESTS_PER_SUITE 64
#endif

/* Public API --------------------------------------------------------------- */
#define TEST(SUITE, NAME)                                                                                                        \
	__pragma(data_seg(push));                                                                                                    \
	__pragma(section("rktest$data", read));                                                                                      \
	void SUITE##_##NAME##_impl(void);                                                                                            \
	const rktest_macro_data_t SUITE##_##NAME##_meta = {                                                                          \
		.test_name = #NAME,                                                                                                      \
		.suite_name = #SUITE,                                                                                                    \
		.func = &SUITE##_##NAME##_impl                                                                                           \
	};                                                                                                                           \
	__declspec(allocate("rktest$data")) const rktest_macro_data_t* const SUITE##_##NAME##_meta##_##ptr = &SUITE##_##NAME##_meta; \
	__pragma(data_seg(pop));                                                                                                     \
	void SUITE##_##NAME##_impl(void)

#define EXPECT_EQ(lhs, rhs)
#define EXPECT_GT(lhs, rhs)
#define EXPECT_TRUE(lhs)
#define EXPECT_FALSE(lhs)

int rktest_main(void);

/* Test runner internals ---------------------------------------------------- */
/**
 * Collects all the information from a TEST() macro
 *
 * Instances of the struct are stored locally in the unit test files. Pointers
 * to instances of this struct are collected and stored in the `rktest$data`
 * memory section.
 */
typedef struct {
	const char* test_name;
	const char* suite_name;
	void (*func)(void);
} rktest_macro_data_t;

inline bool rktest_colors_enabled(void);

#define RKTEST_COLOR_GREEN (rktest_colors_enabled() ? "\033[32m" : "")
#define RKTEST_COLOR_RED (rktest_colors_enabled() ? "\033[31m" : "")
#define RKTEST_COLOR_YELLOW (rktest_colors_enabled() ? "\033[33m" : "")
#define RKTEST_COLOR_RESET (rktest_colors_enabled() ? "\033[0m" : "")

#define rktest_printf_green(...)      \
	printf("%s", RKTEST_COLOR_GREEN); \
	printf(__VA_ARGS__);              \
	printf("%s", RKTEST_COLOR_RESET)

#define rktest_printf_red(...)      \
	printf("%s", RKTEST_COLOR_RED); \
	printf(__VA_ARGS__);            \
	printf("%s", RKTEST_COLOR_RESET)

#define rktest_printf_yellow(...)      \
	printf("%s", RKTEST_COLOR_YELLOW); \
	printf(__VA_ARGS__);               \
	printf("%s", RKTEST_COLOR_RESET)

#define rktest_printf_test_step(prefix_str, ...) \
	rktest_printf_green(prefix_str);             \
	printf(__VA_ARGS__);

#endif /* RKTEST_H */
