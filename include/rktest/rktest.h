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
#include <stdio.h>
#include <string.h>

#ifndef RKTEST_H
#define RKTEST_H

// RK Test is a small unit test library for C99 with an interface heavily based
// on Google Test, featuring self registering tests.

/* Config variables --------------------------------------------------------- */
// Define these variables before including rktest.h to configure the max number
// of supported test suites and max number of unit tests per suite.

#ifndef RKTEST_MAX_NUM_TEST_SUITES
#define RKTEST_MAX_NUM_TEST_SUITES ((size_t)64)
#endif

#ifndef RKTEST_MAX_NUM_TESTS_PER_SUITE
#define RKTEST_MAX_NUM_TESTS_PER_SUITE ((size_t)64)
#endif

/* Public API --------------------------------------------------------------- */
int rktest_main(int argc, const char* argv[]);

#define TEST(SUITE, NAME)                                                              \
	void SUITE##_##NAME##_impl(void);                                                  \
	const rktest_test_t SUITE##_##NAME##_data = {                                      \
		.suite_name = #SUITE,                                                          \
		.test_name = #NAME,                                                            \
		.run = &SUITE##_##NAME##_impl                                                  \
	};                                                                                 \
	ADD_TO_MEMORY_SECTION_BEGIN                                                        \
	const rktest_test_t* const SUITE##_##NAME##_data##_##ptr = &SUITE##_##NAME##_data; \
	ADD_TO_MEMORY_SECTION_END                                                          \
	void SUITE##_##NAME##_impl(void)

// TODO:
// [x] EXPECT_*
// [x] EXPECT_LONG*
//
// [x] EXPECT_STREQ
// [x] EXPECT_STRNE
// [ ] EXPECT_STRCASEEQ
// [ ] EXPECT_STRCASENE
//
// [ ] EXPECT_FLOAT_EQ
// [ ] EXPECT_DOUBLE_EQ
// [ ] EXPECT_NEAR

/* Bool checks */
#define EXPECT_TRUE(expr) RKTEST_CHECK_BOOL(expr, true, RKTEST_CHECK_EXPECT, " ")
#define EXPECT_FALSE(lhs) RKTEST_CHECK_BOOL(lhs, false, RKTEST_CHECK_EXPECT, " ")

#define ASSERT_TRUE(expr) RKTEST_CHECK_BOOL(expr, true, RKTEST_CHECK_ASSERT, " ")
#define ASSERT_FALSE(lhs) RKTEST_CHECK_BOOL(lhs, false, RKTEST_CHECK_ASSERT, " ")

#define EXPECT_TRUE_INFO(expr, ...) RKTEST_CHECK_BOOL(expr, true, RKTEST_CHECK_EXPECT, __VA_ARGS__)
#define EXPECT_FALSE_INFO(lhs, ...) RKTEST_CHECK_BOOL(lhs, false, RKTEST_CHECK_EXPECT, __VA_ARGS__)

#define ASSERT_TRUE_INFO(expr, ...) RKTEST_CHECK_BOOL(expr, true, RKTEST_CHECK_ASSERT, __VA_ARGS__)
#define ASSERT_FALSE_INFO(lhs, ...) RKTEST_CHECK_BOOL(lhs, false, RKTEST_CHECK_ASSERT, __VA_ARGS__)

/* Integer checks */
#define EXPECT_EQ(lhs, rhs) RKTEST_CHECK_EQ(int, "%d", lhs, rhs, RKTEST_CHECK_EXPECT, " ")
#define EXPECT_NE(lhs, rhs) RKTEST_CHECK_CMP(int, "%d", lhs, rhs, !=, RKTEST_CHECK_EXPECT, " ")
#define EXPECT_LT(lhs, rhs) RKTEST_CHECK_CMP(int, "%d", lhs, rhs, <, RKTEST_CHECK_EXPECT, " ")
#define EXPECT_LE(lhs, rhs) RKTEST_CHECK_CMP(int, "%d", lhs, rhs, <=, RKTEST_CHECK_EXPECT, " ")
#define EXPECT_GT(lhs, rhs) RKTEST_CHECK_CMP(int, "%d", lhs, rhs, >, RKTEST_CHECK_EXPECT, " ")
#define EXPECT_GE(lhs, rhs) RKTEST_CHECK_CMP(int, "%d", lhs, rhs, >=, RKTEST_CHECK_EXPECT, " ")

#define ASSERT_EQ(lhs, rhs) RKTEST_CHECK_EQ(int, "%d", lhs, rhs, RKTEST_CHECK_ASSERT, " ")
#define ASSERT_NE(lhs, rhs) RKTEST_CHECK_CMP(int, "%d", lhs, rhs, !=, RKTEST_CHECK_ASSERT, " ")
#define ASSERT_LT(lhs, rhs) RKTEST_CHECK_CMP(int, "%d", lhs, rhs, <, RKTEST_CHECK_ASSERT, " ")
#define ASSERT_LE(lhs, rhs) RKTEST_CHECK_CMP(int, "%d", lhs, rhs, <=, RKTEST_CHECK_ASSERT, " ")
#define ASSERT_GT(lhs, rhs) RKTEST_CHECK_CMP(int, "%d", lhs, rhs, >, RKTEST_CHECK_ASSERT, " ")
#define ASSERT_GE(lhs, rhs) RKTEST_CHECK_CMP(int, "%d", lhs, rhs, >=, RKTEST_CHECK_ASSERT, " ")

#define EXPECT_EQ_INFO(lhs, rhs, ...) RKTEST_CHECK_EQ(int, "%d", lhs, rhs, RKTEST_CHECK_EXPECT, __VA_ARGS__)
#define EXPECT_NE_INFO(lhs, rhs, ...) RKTEST_CHECK_CMP(int, "%d", lhs, rhs, !=, RKTEST_CHECK_EXPECT, __VA_ARGS__)
#define EXPECT_LT_INFO(lhs, rhs, ...) RKTEST_CHECK_CMP(int, "%d", lhs, rhs, <, RKTEST_CHECK_EXPECT, __VA_ARGS__)
#define EXPECT_LE_INFO(lhs, rhs, ...) RKTEST_CHECK_CMP(int, "%d", lhs, rhs, <=, RKTEST_CHECK_EXPECT, __VA_ARGS__)
#define EXPECT_GT_INFO(lhs, rhs, ...) RKTEST_CHECK_CMP(int, "%d", lhs, rhs, >, RKTEST_CHECK_EXPECT, __VA_ARGS__)
#define EXPECT_GE_INFO(lhs, rhs, ...) RKTEST_CHECK_CMP(int, "%d", lhs, rhs, >=, RKTEST_CHECK_EXPECT, __VA_ARGS__)

#define ASSERT_EQ_INFO(lhs, rhs, ...) RKTEST_CHECK_EQ(int, "%d", lhs, rhs, RKTEST_CHECK_ASSERT, __VA_ARGS__)
#define ASSERT_NE_INFO(lhs, rhs, ...) RKTEST_CHECK_CMP(int, "%d", lhs, rhs, !=, RKTEST_CHECK_ASSERT, __VA_ARGS__)
#define ASSERT_LT_INFO(lhs, rhs, ...) RKTEST_CHECK_CMP(int, "%d", lhs, rhs, <, RKTEST_CHECK_ASSERT, __VA_ARGS__)
#define ASSERT_LE_INFO(lhs, rhs, ...) RKTEST_CHECK_CMP(int, "%d", lhs, rhs, <=, RKTEST_CHECK_ASSERT, __VA_ARGS__)
#define ASSERT_GT_INFO(lhs, rhs, ...) RKTEST_CHECK_CMP(int, "%d", lhs, rhs, >, RKTEST_CHECK_ASSERT, __VA_ARGS__)
#define ASSERT_GE_INFO(lhs, rhs, ...) RKTEST_CHECK_CMP(int, "%d", lhs, rhs, >=, RKTEST_CHECK_ASSERT, __VA_ARGS__)

/* Long checks */
#define EXPECT_LONG_EQ(lhs, rhs) RKTEST_CHECK_EQ(long, "%ld", lhs, rhs, RKTEST_CHECK_EXPECT, " ")
#define EXPECT_LONG_NE(lhs, rhs) RKTEST_CHECK_CMP(long, "%ld", lhs, rhs, !=, RKTEST_CHECK_EXPECT, " ")
#define EXPECT_LONG_LT(lhs, rhs) RKTEST_CHECK_CMP(long, "%ld", lhs, rhs, <, RKTEST_CHECK_EXPECT, " ")
#define EXPECT_LONG_LE(lhs, rhs) RKTEST_CHECK_CMP(long, "%ld", lhs, rhs, <=, RKTEST_CHECK_EXPECT, " ")
#define EXPECT_LONG_GT(lhs, rhs) RKTEST_CHECK_CMP(long, "%ld", lhs, rhs, >, RKTEST_CHECK_EXPECT, " ")
#define EXPECT_LONG_GE(lhs, rhs) RKTEST_CHECK_CMP(long, "%ld", lhs, rhs, >=, RKTEST_CHECK_EXPECT, " ")

#define ASSERT_LONG_EQ(lhs, rhs) RKTEST_CHECK_EQ(long, "%ld", lhs, rhs, RKTEST_CHECK_ASSERT, " ")
#define ASSERT_LONG_NE(lhs, rhs) RKTEST_CHECK_CMP(long, "%ld", lhs, rhs, !=, RKTEST_CHECK_ASSERT, " ")
#define ASSERT_LONG_LT(lhs, rhs) RKTEST_CHECK_CMP(long, "%ld", lhs, rhs, <, RKTEST_CHECK_ASSERT, " ")
#define ASSERT_LONG_LE(lhs, rhs) RKTEST_CHECK_CMP(long, "%ld", lhs, rhs, <=, RKTEST_CHECK_ASSERT, " ")
#define ASSERT_LONG_GT(lhs, rhs) RKTEST_CHECK_CMP(long, "%ld", lhs, rhs, >, RKTEST_CHECK_ASSERT, " ")
#define ASSERT_LONG_GE(lhs, rhs) RKTEST_CHECK_CMP(long, "%ld", lhs, rhs, >=, RKTEST_CHECK_ASSERT, " ")

#define EXPECT_LONG_EQ_INFO(lhs, rhs, ...) RKTEST_CHECK_EQ(long, "%ld", lhs, rhs, RKTEST_CHECK_EXPECT, __VA_ARGS__)
#define EXPECT_LONG_NE_INFO(lhs, rhs, ...) RKTEST_CHECK_CMP(long, "%ld", lhs, rhs, !=, RKTEST_CHECK_EXPECT, __VA_ARGS__)
#define EXPECT_LONG_LT_INFO(lhs, rhs, ...) RKTEST_CHECK_CMP(long, "%ld", lhs, rhs, <, RKTEST_CHECK_EXPECT, __VA_ARGS__)
#define EXPECT_LONG_LE_INFO(lhs, rhs, ...) RKTEST_CHECK_CMP(long, "%ld", lhs, rhs, <=, RKTEST_CHECK_EXPECT, __VA_ARGS__)
#define EXPECT_LONG_GT_INFO(lhs, rhs, ...) RKTEST_CHECK_CMP(long, "%ld", lhs, rhs, >, RKTEST_CHECK_EXPECT, __VA_ARGS__)
#define EXPECT_LONG_GE_INFO(lhs, rhs, ...) RKTEST_CHECK_CMP(long, "%ld", lhs, rhs, >=, RKTEST_CHECK_EXPECT, __VA_ARGS__)

#define ASSERT_LONG_EQ_INFO(lhs, rhs, ...) RKTEST_CHECK_EQ(long, "%ld", lhs, rhs, RKTEST_CHECK_ASSERT, __VA_ARGS__)
#define ASSERT_LONG_NE_INFO(lhs, rhs, ...) RKTEST_CHECK_CMP(long, "%ld", lhs, rhs, !=, RKTEST_CHECK_ASSERT, __VA_ARGS__)
#define ASSERT_LONG_LT_INFO(lhs, rhs, ...) RKTEST_CHECK_CMP(long, "%ld", lhs, rhs, <, RKTEST_CHECK_ASSERT, __VA_ARGS__)
#define ASSERT_LONG_LE_INFO(lhs, rhs, ...) RKTEST_CHECK_CMP(long, "%ld", lhs, rhs, <=, RKTEST_CHECK_ASSERT, __VA_ARGS__)
#define ASSERT_LONG_GT_INFO(lhs, rhs, ...) RKTEST_CHECK_CMP(long, "%ld", lhs, rhs, >, RKTEST_CHECK_ASSERT, __VA_ARGS__)
#define ASSERT_LONG_GE_INFO(lhs, rhs, ...) RKTEST_CHECK_CMP(long, "%ld", lhs, rhs, >=, RKTEST_CHECK_ASSERT, __VA_ARGS__)

/* String checks */
#define EXPECT_STREQ(lhs, rhs) RKTEST_CHECK_STREQ(lhs, rhs, RKTEST_CHECK_EXPECT, RKTEST_MATCH_CASE, " ")
#define EXPECT_STRNE(lhs, rhs) RKTEST_CHECK_STRNE(lhs, rhs, RKTEST_CHECK_EXPECT, RKTEST_MATCH_CASE, " ")
#define EXPECT_CASE_STREQ(lhs, rhs) RKTEST_CHECK_STREQ(lhs, rhs, RKTEST_CHECK_EXPECT, RKTEST_CASE_INSENSETIVE, " ")
#define EXPECT_CASE_STRNE(lhs, rhs) RKTEST_CHECK_STRNE(lhs, rhs, RKTEST_CHECK_EXPECT, RKTEST_CASE_INSENSETIVE, " ")

#define EXPECT_STREQ_INFO(lhs, rhs, ...) RKTEST_CHECK_STREQ(lhs, rhs, RKTEST_CHECK_EXPECT, RKTEST_MATCH_CASE, __VA_ARGS__)
#define EXPECT_STRNE_INFO(lhs, rhs, ...) RKTEST_CHECK_STRNE(lhs, rhs, RKTEST_CHECK_EXPECT, RKTEST_MATCH_CASE, __VA_ARGS__)
#define EXPECT_CASE_STREQ_INFO(lhs, rhs, ...) RKTEST_CHECK_STREQ(lhs, rhs, RKTEST_CHECK_EXPECT, RKTEST_CASE_INSENSETIVE, __VA_ARGS__)
#define EXPECT_CASE_STRNE_INFO(lhs, rhs, ...) RKTEST_CHECK_STRNE(lhs, rhs, RKTEST_CHECK_EXPECT, RKTEST_CASE_INSENSETIVE, __VA_ARGS__)

#define ASSERT_STREQ(lhs, rhs) RKTEST_CHECK_STREQ(lhs, rhs, RKTEST_CHECK_ASSERT, RKTEST_MATCH_CASE, " ")
#define ASSERT_STRNE(lhs, rhs) RKTEST_CHECK_STRNE(lhs, rhs, RKTEST_CHECK_ASSERT, RKTEST_MATCH_CASE, " ")
#define ASSERT_CASE_STREQ(lhs, rhs) RKTEST_CHECK_STREQ(lhs, rhs, RKTEST_CHECK_ASSERT, RKTEST_CASE_INSENSETIVE, " ")
#define ASSERT_CASE_STRNE(lhs, rhs) RKTEST_CHECK_STRNE(lhs, rhs, RKTEST_CHECK_ASSERT, RKTEST_CASE_INSENSETIVE, " ")

#define ASSERT_STREQ_INFO(lhs, rhs, ...) RKTEST_CHECK_STREQ(lhs, rhs, RKTEST_CHECK_ASSERT, RKTEST_MATCH_CASE, __VA_ARGS__)
#define ASSERT_STRNE_INFO(lhs, rhs, ...) RKTEST_CHECK_STRNE(lhs, rhs, RKTEST_CHECK_ASSERT, RKTEST_MATCH_CASE, __VA_ARGS__)
#define ASSERT_CASE_STREQ_INFO(lhs, rhs, ...) RKTEST_CHECK_STREQ(lhs, rhs, RKTEST_CHECK_ASSERT, RKTEST_CASE_INSENSETIVE, __VA_ARGS__)
#define ASSERT_CASE_STRNE_INFO(lhs, rhs, ...) RKTEST_CHECK_STRNE(lhs, rhs, RKTEST_CHECK_ASSERT, RKTEST_CASE_INSENSETIVE, __VA_ARGS__)

/* Test runner internals ---------------------------------------------------- */
/* Test registration */
#if defined(_MSC_VER)
#define ADD_TO_MEMORY_SECTION_BEGIN         \
	__pragma(data_seg(push));               \
	__pragma(section("rktest$data", read)); \
	__declspec(allocate("rktest$data"))
#elif defined(__APPLE__)
#define ADD_TO_MEMORY_SECTION_BEGIN __attribute__((used, section("__DATA,rktest")))
#elif defined(__unix__)
#define ADD_TO_MEMORY_SECTION_BEGIN __attribute__((used, section("rktest")))
#endif

#if defined(_MSC_VER)
#define ADD_TO_MEMORY_SECTION_END __pragma(data_seg(pop));
#elif defined(__APPLE__)
#define ADD_TO_MEMORY_SECTION_END
#elif defined(__unix__)
#define ADD_TO_MEMORY_SECTION_END
#else
#error Trying to compile RK Test on an unsupported platform.
#endif

// Collects all the information from a TEST() macro
//
// Instances of the struct are stored locally in the unit test files. Pointers
// to instances of this struct are collected and stored in the `rktest$data`
// memory section.
typedef struct {
	const char* suite_name;
	const char* test_name;
	void (*run)(void);
} rktest_test_t;

/* Assertions */
#define RKTEST_CHECK_EXPECT false
#define RKTEST_CHECK_ASSERT true

#define RKTEST_CASE_INSENSETIVE false
#define RKTEST_MATCH_CASE true

void rktest_fail_current_test(void);
bool rktest_string_is_number(const char* str);
int rktest_strcasecmp(const char* lhs, const char* rhs);

#define RKTEST_CHECK_BOOL(actual, expected, is_assert, ...)                        \
	do {                                                                           \
		const bool actual_val = actual;                                            \
		const bool expected_val = expected;                                        \
		if (actual_val != expected_val) {                                          \
			printf("%s(%d): error: Value of: %s:\n", __FILE__, __LINE__, #actual); \
			printf("  Actual: %s\n", actual_val ? "true" : "false");               \
			printf("Expected: %s\n", expected_val ? "true" : "false");             \
			printf(__VA_ARGS__);                                                   \
			printf("\n");                                                          \
			rktest_fail_current_test();                                            \
			if (is_assert) {                                                       \
				return;                                                            \
			}                                                                      \
		}                                                                          \
	} while (0)

#define RKTEST_CHECK_EQ(type, fmt, lhs, rhs, is_assert, ...)                                   \
	do {                                                                                       \
		const type lhs_val = lhs;                                                              \
		const type rhs_val = rhs;                                                              \
		if (lhs_val != rhs_val) {                                                              \
			printf("%s(%d): error: Expected equality of these values:\n", __FILE__, __LINE__); \
			printf("  %s\n", #lhs);                                                            \
			const bool lhs_is_literal = rktest_string_is_number(#lhs);                         \
			if (!lhs_is_literal)                                                               \
				printf("    Which is: " fmt "\n", lhs_val);                                    \
			printf("  %s\n", #rhs);                                                            \
			const bool rhs_is_literal = rktest_string_is_number(#rhs);                         \
			if (!rhs_is_literal)                                                               \
				printf("    Which is: " fmt "\n", rhs_val);                                    \
			printf(__VA_ARGS__);                                                               \
			printf("\n");                                                                      \
			rktest_fail_current_test();                                                        \
			if (is_assert) {                                                                   \
				return;                                                                        \
			}                                                                                  \
		}                                                                                      \
	} while (0)

#define RKTEST_CHECK_CMP(type, fmt, lhs, rhs, op, is_assert, ...)                                                                                \
	do {                                                                                                                                         \
		const type lhs_val = lhs;                                                                                                                \
		const type rhs_val = rhs;                                                                                                                \
		if (!(lhs_val op rhs_val)) {                                                                                                             \
			printf("%s(%d): error: Expected (%s) %s (%s), actual: " fmt " vs " fmt "\n", __FILE__, __LINE__, #lhs, #op, #rhs, lhs_val, rhs_val); \
			printf(__VA_ARGS__);                                                                                                                 \
			printf("\n");                                                                                                                        \
			rktest_fail_current_test();                                                                                                          \
			if (is_assert) {                                                                                                                     \
				return;                                                                                                                          \
			}                                                                                                                                    \
		}                                                                                                                                        \
	} while (0)

#define RKTEST_CHECK_STREQ(lhs, rhs, is_assert, match_case, ...)                                         \
	do {                                                                                                 \
		const char* lhs_val = lhs;                                                                       \
		const char* rhs_val = rhs;                                                                       \
		if (match_case ? (strcmp(lhs_val, rhs_val) != 0) : (rktest_strcasecmp(lhs_val, rhs_val) != 0)) { \
			printf("%s(%d): error: Expected equality of these values:\n", __FILE__, __LINE__);           \
			printf("  %s\n", #lhs);                                                                      \
			const bool lhs_is_literal = (#lhs)[0] == '"';                                                \
			if (!lhs_is_literal)                                                                         \
				printf("    Which is: %s\n", lhs_val);                                                   \
			printf("  %s\n", #rhs);                                                                      \
			const bool rhs_is_literal = (#rhs)[0] == '"';                                                \
			if (!rhs_is_literal)                                                                         \
				printf("    Which is: %s\n", rhs_val);                                                   \
			if (!match_case)                                                                             \
				printf("Ignoring case\n");                                                               \
			printf(__VA_ARGS__);                                                                         \
			printf("\n");                                                                                \
			rktest_fail_current_test();                                                                  \
			if (is_assert) {                                                                             \
				return;                                                                                  \
			}                                                                                            \
		}                                                                                                \
	} while (0)

#define RKTEST_CHECK_STRNE(lhs, rhs, is_assert, match_case, ...)                                         \
	do {                                                                                                 \
		const char* lhs_val = lhs;                                                                       \
		const char* rhs_val = rhs;                                                                       \
		if (match_case ? (strcmp(lhs_val, rhs_val) == 0) : (rktest_strcasecmp(lhs_val, rhs_val) == 0)) { \
			printf("%s(%d): error: Expected (%s) != (%s)", __FILE__, __LINE__, #lhs, #rhs);              \
			if (!match_case)                                                                             \
				printf(" (ignoring case)");                                                              \
			printf(", actual: \"%s\" vs \"%s\"\n", lhs_val, rhs_val);                                    \
			printf(__VA_ARGS__);                                                                         \
			printf("\n");                                                                                \
			rktest_fail_current_test();                                                                  \
			if (is_assert) {                                                                             \
				return;                                                                                  \
			}                                                                                            \
		}                                                                                                \
	} while (0)

/* Logging */
bool rktest_colors_enabled(void);

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

#define rktest_log_info(prefix_str, ...) \
	rktest_printf_green(prefix_str);     \
	printf(__VA_ARGS__);

#define rktest_log_error(prefix_str, ...) \
	rktest_printf_red(prefix_str);        \
	printf(__VA_ARGS__);

#endif /* RKTEST_H */
