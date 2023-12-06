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
#include "rktest/rktest.h"

#include <ctype.h>
#include <float.h>
#include <math.h>
#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER
#include <windows.h>
#elif defined(__MACH__)
#include <mach/mach_time.h>
#else
#include <time.h>
#endif

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wmissing-braces"
#endif

#define RKTEST_MAX_NUM_TESTS (RKTEST_MAX_NUM_TEST_SUITES * RKTEST_MAX_NUM_TESTS_PER_SUITE)

#define foreach(type_ptr, iter, array, array_len) \
	for (type_ptr iter = &array[0]; iter != &array[array_len]; iter++)

typedef enum {
	RKTEST_RESULT_OK,
	RKTEST_RESULT_ERROR,
} rktest_result_t;

typedef struct {
	const char* name;
	rktest_test_t tests[RKTEST_MAX_NUM_TESTS_PER_SUITE];
	size_t num_tests;
} rktest_suite_t;

typedef struct {
	rktest_suite_t test_suites[RKTEST_MAX_NUM_TEST_SUITES];
	size_t num_test_suites;
	size_t total_num_tests;
} rktest_environment_t;

typedef struct {
	size_t num_passed_tests;
	rktest_test_t failed_tests[RKTEST_MAX_NUM_TESTS];
	size_t num_failed_tests;
} rktest_report_t;

/* Timer type */
typedef int rktest_millis_t;

#if defined(WIN32)
typedef struct {
	double pc_freq;
	__int64 start;
} rktest_timer_t;
#elif defined(__MACH__)
typedef struct {
	mach_timebase_info_data_t timebase_info;
	uint64_t start;
	uint64_t end;
} rktest_timer_t;
#else
typedef struct {
	struct timespec start;
	struct timespec end;
} rktest_timer_t;
#endif

/* ------------------------- Timer implementation -------------------------- */
#if defined(WIN32)
rktest_timer_t rktest_timer_start(void) {
	rktest_timer_t timer;

	LARGE_INTEGER li;
	QueryPerformanceFrequency(&li);

	timer.pc_freq = (double)(li.QuadPart) / 1000.0;

	QueryPerformanceCounter(&li);
	timer.start = li.QuadPart;

	return timer;
}
#elif defined(__MACH__)
rktest_timer_t rktest_timer_start(void) {
	rktest_timer_t timer;
	mach_timebase_info(&timer.timebase_info);
	timer.start = mach_absolute_time();
	return timer;
}
#else
rktest_timer_t rktest_timer_start(void) {
	rktest_timer_t timer;
	clock_gettime(CLOCK_MONOTONIC, &timer.start);
	return timer;
}
#endif

#if defined(WIN32)
rktest_millis_t rktest_timer_stop(rktest_timer_t* timer) {
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	const int ms = (int)round((li.QuadPart - timer->start) / timer->pc_freq);
	return ms;
}
#elif defined(__MACH__)
rktest_millis_t rktest_timer_stop(rktest_timer_t* timer) {
	timer->end = mach_absolute_time();
	rktest_millis_t ms = (rktest_millis_t)((timer->end - timer->start) * timer->timebase_info.numer / timer->timebase_info.denom / 1000000);
	return ms;
}
#else
rktest_millis_t rktest_timer_stop(rktest_timer_t* timer) {
	clock_gettime(CLOCK_MONOTONIC, &timer->end);
	rktest_millis_t ms = 0;
	ms += (rktest_millis_t)((timer->end.tv_sec - timer->start.tv_sec) * 1000.0); // seconds
	ms += (rktest_millis_t)((timer->end.tv_nsec - timer->start.tv_nsec) / 1000.0); // milliseconds
	return ms;
}
#endif

/* Declare memory section to store test data in */
// This is based on the following article: https://christophercrouzet.com/blog/dev/rexo-part-2
#if defined(_MSC_VER)
__pragma(section("rktest$begin", read));
__pragma(section("rktest$data", read));
__pragma(section("rktest$end", read));
__declspec(allocate("rktest$begin")) extern const rktest_test_t* const test_data_begin = NULL;
__declspec(allocate("rktest$end")) extern const rktest_test_t* const test_data_end = NULL;
#elif defined(__APPLE__)
extern const rktest_test_t* const __start_rktest __asm("section$start$__DATA$rktest");
extern const rktest_test_t* const __stop_rktest __asm("section$end$__DATA$rktest");
__attribute__((used, section("__DATA,rktest"))) const rktest_test_t* const dummy = NULL;
#elif defined(__unix__)
extern const rktest_test_t* const __start_rktest;
extern const rktest_test_t* const __stop_rktest;
__attribute__((used, section("rktest"))) const rktest_test_t* const dummy = NULL;
#endif

#if defined(_MSC_VER)
#define TEST_DATA_BEGIN (&test_data_begin + 1)
#define TEST_DATA_END (&test_data_end)
#elif defined(__unix__) || defined(__APPLE__)
#define TEST_DATA_BEGIN (&__start_rktest)
#define TEST_DATA_END (&__stop_rktest)
#endif

/* -------------------- Public function implementations -------------------- */
static bool g_colors_enabled = false;
static bool g_current_test_failed = false;

bool rktest_colors_enabled(void) {
	return g_colors_enabled;
}

void rktest_fail_current_test(void) {
	g_current_test_failed = true;
}

bool rktest_string_is_number(const char* str) {
	for (int i = 0; str[i] != '\0'; i++) {
		if (!isdigit(str[i])) {
			return false;
		}
	}
	return true;
}

// based on https://stackoverflow.com/a/34873406/3157744
int rktest_strcasecmp(const char* lhs, const char* rhs) {
	while (*lhs && (tolower(*lhs) == tolower(*rhs))) {
		lhs++;
		rhs++;
	}
	return *(const unsigned char*)lhs - *(const unsigned char*)rhs;
}

static float next_4_ulp_float(float x) {
	for (int i = 0; i < 4; i++) {
		x = nextafterf(x, FLT_MAX);
	}
	return x;
}

static float prev_4_ulp_float(float x) {
	for (int i = 0; i < 4; i++) {
		x = nextafterf(x, -FLT_MAX);
	}
	return x;
}

static double next_4_ulp_double(double x) {
	for (int i = 0; i < 4; i++) {
		x = nextafter(x, DBL_MAX);
	}
	return x;
}

static double prev_4_ulp_double(double x) {
	for (int i = 0; i < 4; i++) {
		x = nextafter(x, -DBL_MAX);
	}
	return x;
}

bool rktest_floats_within_4_ulp(float lhs, float rhs) {
	return prev_4_ulp_float(rhs) <= lhs && lhs <= next_4_ulp_float(rhs);
}

bool rktest_doubles_within_4_ulp(double lhs, double rhs) {
	return prev_4_ulp_double(rhs) <= lhs && lhs <= next_4_ulp_double(rhs);
}

/* ------------------------ Platform initialization ------------------------ */
#ifdef _MSC_VER
static rktest_result_t enable_windows_virtual_terminal(void) {
	// Set output mode to handle virtual terminal sequences
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "Error: GetStdHandle returned INVALID_HANDLE_VALUE\n");
		return RKTEST_RESULT_ERROR;
	}

	DWORD dwMode = 0;
	if (!GetConsoleMode(hOut, &dwMode)) {
		fprintf(stderr, "Error: GetConsoleMode failed\n");
		return RKTEST_RESULT_ERROR;
	}

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hOut, dwMode)) {
		fprintf(stderr, "Error: SetConsoleMode to ENABLE_VIRTUAL_TERMINAL_PROCESSING failed\n");
		return RKTEST_RESULT_ERROR;
	}

	return RKTEST_RESULT_OK;
}
#endif // WIN32

static void initialize(int argc, const char* argv[]) {
	g_colors_enabled = true;

	if (argc > 1 && strcmp(argv[1], "--rktest-color=no") == 0) {
		g_colors_enabled = false;
	}

#ifdef _MSC_VER
	if (g_colors_enabled) {
		const rktest_result_t enable_virtual_term = enable_windows_virtual_terminal();
		if (enable_virtual_term != RKTEST_RESULT_OK) {
			fprintf(stderr, "Error: could not initialize color output\n");
			g_colors_enabled = false;
		}
	}
#endif // WIN32
}

/* ------------------------- RKTest implementation ------------------------- */
static rktest_suite_t* find_suite_with_name(rktest_suite_t* suites, size_t num_suites, const char* suite_name) {
	foreach (rktest_suite_t*, suite, suites, num_suites) {
		if (strcmp(suite->name, suite_name) == 0) {
			return suite;
		}
	}
	return NULL;
}

static rktest_suite_t* add_new_suite(rktest_environment_t* env, const char* suite_name) {
	rktest_suite_t* suite = &env->test_suites[env->num_test_suites++];
	*suite = (rktest_suite_t) {
		.name = suite_name,
		.num_tests = 0,
		.tests = { 0 },
	};
	return suite;
}

static rktest_suite_t* find_or_add_suite(rktest_environment_t* env, const char* suite_name) {
	rktest_suite_t* suite = find_suite_with_name(env->test_suites, env->num_test_suites, suite_name);
	suite = suite ? suite : add_new_suite(env, suite_name);
	return suite;
}

// Loop through the entirety of the `rkdata` memory section, including padding.
// If the iterator `it` points to null, it's padding and we skip it.
// If it's non-null, we have a test and push it into `tests`.
static rktest_environment_t* setup_test_env(void) {
	rktest_environment_t* env = malloc(sizeof(rktest_environment_t));
	*env = (rktest_environment_t) {
		.test_suites = { 0 },
		.num_test_suites = 0,
		.total_num_tests = 0,
	};

	for (const rktest_test_t* const* it = TEST_DATA_BEGIN; it != TEST_DATA_END; it++) {
		if (*it == NULL) {
			continue;
		}

		const rktest_test_t* const test = *it;

		if (env->num_test_suites == RKTEST_MAX_NUM_TEST_SUITES) {
			fprintf(stderr, "Error: number of test suites is greater than RKTEST_MAX_NUM_TEST_SUITES (%zu)."
							"See the `Config variables` section of rktest.h\n",
					RKTEST_MAX_NUM_TEST_SUITES);
			exit(1);
		}

		rktest_suite_t* suite = find_or_add_suite(env, test->suite_name);

		if (suite->num_tests == RKTEST_MAX_NUM_TESTS_PER_SUITE) {
			fprintf(stderr, "Error: number of tests in suite %s is greater than RKTEST_MAX_NUM_TESTS_PER_SUITE (%zu)."
							"See the `Config variables` section of rktest.h\n",
					suite->name,
					RKTEST_MAX_NUM_TESTS_PER_SUITE);
			exit(1);
		}

		// add test to suite
		env->total_num_tests++;
		suite->tests[suite->num_tests++] = *test;
	}

	// return env;
	return env;
}

static bool run_test(const rktest_test_t* test) {
	rktest_log_info("[ RUN      ] ", "%s.%s \n", test->suite_name, test->test_name);

	rktest_timer_t test_timer = rktest_timer_start();
	test->run();
	rktest_millis_t test_time_ms = rktest_timer_stop(&test_timer);

	const bool test_passed = !g_current_test_failed;
	g_current_test_failed = false;

	if (test_passed) {
		rktest_log_info("[       OK ] ", "%s.%s (%d ms)\n", test->suite_name, test->test_name, test_time_ms);
	} else {
		rktest_log_error("[  FAILED  ] ", "%s.%s (%d ms)\n", test->suite_name, test->test_name, test_time_ms);
	}

	return test_passed;
}

static rktest_report_t* run_all_tests(rktest_environment_t* env) {
	rktest_report_t* report = malloc(sizeof(rktest_report_t));
	*report = (rktest_report_t) {
		.failed_tests = { 0 },
		.num_failed_tests = 0,
		.num_passed_tests = 0,
	};

	foreach (rktest_suite_t*, suite, env->test_suites, env->num_test_suites) {
		rktest_log_info("[----------] ", "%zu tests from %s\n", suite->num_tests, suite->name);
		rktest_timer_t suite_timer = rktest_timer_start();
		foreach (rktest_test_t*, test, suite->tests, suite->num_tests) {
			const bool test_passed = run_test(test);
			if (test_passed) {
				report->num_passed_tests++;
			} else {
				report->failed_tests[report->num_failed_tests] = *test;
				report->num_failed_tests++;
			}
		}
		rktest_millis_t suite_time_ms = rktest_timer_stop(&suite_timer);
		rktest_log_info("[----------] ", "%zu tests from %s (%d ms total)\n", suite->num_tests, suite->name, suite_time_ms);
		printf("\n");
	}

	return report;
}

static void print_failed_tests(rktest_report_t* report) {
	rktest_log_error("[  FAILED  ] ", "%zu tests, listed below:\n", report->num_failed_tests);
	foreach (const rktest_test_t*, failed_test, report->failed_tests, report->num_failed_tests) {
		rktest_log_error("[  FAILED  ] ", "%s.%s\n", failed_test->suite_name, failed_test->test_name);
	}
	printf("\n");
	printf(" %zu FAILED TESTS\n", report->num_failed_tests);
}

int rktest_main(int argc, const char* argv[]) {
	initialize(argc, argv);

	rktest_environment_t* env = setup_test_env();

	rktest_log_info("[==========] ", "Running %zu tests from %zu test suites.\n", env->total_num_tests, env->num_test_suites);
	rktest_log_info("[----------] ", "Global test environment set-up.\n");

	rktest_timer_t total_time_timer = rktest_timer_start();
	rktest_report_t* report = run_all_tests(env);
	rktest_millis_t total_time_ms = rktest_timer_stop(&total_time_timer);

	rktest_log_info("[----------] ", "Global test environment tear-down.\n");
	rktest_log_info("[==========] ", "%zu tests from %zu test suites ran. (%d ms total)\n", env->total_num_tests, env->num_test_suites, total_time_ms);
	rktest_log_info("[  PASSED  ] ", "%zu tests.\n", report->num_passed_tests);

	const bool tests_failed = report->num_failed_tests > 0;
	if (tests_failed) {
		print_failed_tests(report);
	}

	free(report);
	free(env);

	return tests_failed;
}
