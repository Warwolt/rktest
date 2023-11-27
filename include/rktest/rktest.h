#include <stdbool.h>
#include <stddef.h>

// RKTest is a small unit test library for C99 heavily based on Google Test.

/* Public API -------------------------------------------------------------------------------------*/
#define TEST(SUITE, NAME)                                                                                                   \
	__pragma(data_seg(push));                                                                                               \
	__pragma(section("rktest$data", read));                                                                                 \
	void SUITE##_##NAME##_impl(void);                                                                                       \
	const _rktest_test_t SUITE##_##NAME##_data = {                                                                          \
		.name = #NAME,                                                                                                      \
		.suite = #SUITE,                                                                                                    \
		.func = &SUITE##_##NAME##_impl                                                                                      \
	};                                                                                                                      \
	__declspec(allocate("rktest$data")) const _rktest_test_t* const SUITE##_##NAME##_data##_##ptr = &SUITE##_##NAME##_data; \
	__pragma(data_seg(pop));                                                                                                \
	void SUITE##_##NAME##_impl(void)

#define EXPECT_EQ(lhs, rhs)
#define EXPECT_GT(lhs, rhs)
#define EXPECT_TRUE(lhs)
#define EXPECT_FALSE(lhs)

int rktest_main(void);

/* Test runner internals -------------------------------------------------------------------------*/
typedef int _rktest_result_t;
#define _RKTEST_RESULT_OK ((_rktest_result_t)0)
#define _RKTEST_RESULT_ERROR ((_rktest_result_t)1)

typedef struct {
	const char* name;
	const char* suite;
	void (*func)(void);
} _rktest_test_t;

inline bool _rktest_colors_enabled(void);

#define _RKTEST_COLOR_GREEN (_rktest_colors_enabled() ? "\033[32m" : "")
#define _RKTEST_COLOR_RED (_rktest_colors_enabled() ? "\033[31m" : "")
#define _RKTEST_COLOR_YELLOW (_rktest_colors_enabled() ? "\033[33m" : "")
#define _RKTEST_COLOR_RESET (_rktest_colors_enabled() ? "\033[0m" : "")

#define _rktest_printf_green(...)      \
	printf("%s", _RKTEST_COLOR_GREEN); \
	printf(__VA_ARGS__);               \
	printf("%s", _RKTEST_COLOR_RESET)

#define _rktest_printf_red(...)      \
	printf("%s", _RKTEST_COLOR_RED); \
	printf(__VA_ARGS__);             \
	printf("%s", _RKTEST_COLOR_RESET)

#define _rktest_printf_yellow(...)      \
	printf("%s", _RKTEST_COLOR_YELLOW); \
	printf(__VA_ARGS__);                \
	printf("%s", _RKTEST_COLOR_RESET)
