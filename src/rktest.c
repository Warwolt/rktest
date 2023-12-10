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

#ifdef _MSC_VER
#pragma warning(disable: 4996) // needed for strncpy
#endif

/* -------------------------- Types and constants -------------------------- */
#define RKTEST_MAX_NUM_TESTS (RKTEST_MAX_NUM_TEST_SUITES * RKTEST_MAX_NUM_TESTS_PER_SUITE)
#define RKTEST_MAX_FILTER_LENGTH 256

#define foreach(type_ptr, iter, array, array_len) \
	for (type_ptr iter = &array[0]; iter != &array[array_len]; iter++)

typedef enum {
	RKTEST_ENABLE_VTERM_ERROR_INVALID_HANDLE_VALUE,
	RKTEST_ENABLE_VTERM_ERROR_GET_CONSOLE_MODE_FAILED,
	RKTEST_ENABLE_VTERM_ERROR_ENABLE_VIRTUAL_TERMINAL_FAILED,
	RKTEST_ENABLE_VTERM_OK,
} rktest_enable_vterm_result_t;

typedef enum {
	RKTEST_COLOR_MODE_ON,
	RKTEST_COLOR_MODE_OFF,
	RKTEST_COLOR_MODE_AUTO,
} rktest_color_mode_t;

typedef struct {
	rktest_color_mode_t color_mode;
	char test_filter[RKTEST_MAX_FILTER_LENGTH];
	bool print_timestamps_enabled;
} rktest_config_t;

typedef struct {
	const char* name;
	rktest_test_t tests[RKTEST_MAX_NUM_TESTS_PER_SUITE];
	bool test_is_disabled[RKTEST_MAX_NUM_TESTS_PER_SUITE];
	size_t total_num_tests;
	size_t num_disabled_tests;
} rktest_suite_t;

typedef struct {
	rktest_suite_t test_suites[RKTEST_MAX_NUM_TEST_SUITES];
	size_t num_test_suites;
	size_t total_num_filtered_suites;
	size_t total_num_filtered_tests;
	size_t total_num_disabled_tests;
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
	clock_gettime(CLOCK_REALTIME, &timer.start);
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
	clock_gettime(CLOCK_REALTIME, &timer->end);
	rktest_millis_t ms = 0;
	ms += (rktest_millis_t)((timer->end.tv_sec - timer->start.tv_sec) * 1000.0); // seconds
	ms += (rktest_millis_t)((timer->end.tv_nsec - timer->start.tv_nsec) / 1000.0); // milliseconds
	return ms;
}
#endif

/* ---------------------------- String utility ----------------------------- */
static bool string_starts_with(const char* str, const char* prefix) {
	return strncmp(prefix, str, strlen(prefix)) == 0;
}

// Based on "EnhancedMaskTest" function in 7zip source code
// https://github.com/mcmilk/7-Zip/blob/master/CPP/Common/Wildcard.cpp
static bool string_wildcard_match(const char* str, const char* pattern) {
	while (true) {
		if (pattern[0] == 0) {
			return (str[0] == 0);
		}
		if (pattern[0] == '*') {
			if (string_wildcard_match(str, pattern + 1)) {
				return true;
			}
			if (str[0] == 0) {
				return false;
			}
		} else {
			if (pattern[0] == '?') {
				if (str[0] == 0) {
					return false;
				}
			} else if (pattern[0] != str[0]) {
				return false;
			}
			pattern++;
		}
		str++;
	}
}

/* ----------------------- Test case memory storage ------------------------ */
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

/* -------------------- Header function implementations -------------------- */
static bool g_colors_enabled = false;
static bool g_current_test_failed = false;
static bool g_filenames_enabled = true;

bool rktest_colors_enabled(void) {
	return g_colors_enabled;
}

bool rktest_filenames_enabled(void) {
	return g_filenames_enabled;
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

/* ------------------------- RKTest implementation ------------------------- */
static void print_usage(void) {
	// TODO
}

static rktest_config_t parse_args(int argc, const char* argv[]) {
	rktest_config_t config = (rktest_config_t) { 0 };
	config.color_mode = RKTEST_COLOR_MODE_AUTO;
	config.print_timestamps_enabled = true;

	for (int i = 1; i < argc; i++) {
		const char* arg = argv[i];

		if (string_starts_with(arg, "--rktest_color=")) {
			if (strcmp(arg + strlen("--rktest_color="), "yes") == 0) {
				config.color_mode = RKTEST_COLOR_MODE_ON;
			} else if (strcmp(arg + strlen("--rktest_color="), "no") == 0) {
				config.color_mode = RKTEST_COLOR_MODE_OFF;
			} else if (strcmp(arg + strlen("--rktest_color="), "auto") == 0) {
				config.color_mode = RKTEST_COLOR_MODE_AUTO;
			} else {
				fprintf(stderr, "Error: Unrecognized argument %s\n", arg);
				print_usage();
				exit(1);
			}
		}

		else if (string_starts_with(arg, "--rktest_filter=")) {
			const char* filter_pattern = arg + strlen("--rktest_filter=");
			const size_t filter_len = strlen(filter_pattern);
			if (filter_len > RKTEST_MAX_FILTER_LENGTH) {
				fprintf(stderr, "Error: filter pattern too long. Max length is (%d)", RKTEST_MAX_FILTER_LENGTH);
				fprintf(stderr, "filter pattern = \"%s\"", filter_pattern);
				exit(1);
			}
			strncpy(config.test_filter, filter_pattern, filter_len);
		}

		else if (string_starts_with(arg, "--rktest_print_time=")) {
			if (strcmp(arg + strlen("--rktest_print_time="), "0") == 0) {
				config.print_timestamps_enabled = false;
			} else {
				config.print_timestamps_enabled = true;
			}
		}

		else if (string_starts_with(arg, "--rktest_print_filenames=")) {
			if (strcmp(arg + strlen("--rktest_print_filenames="), "0") == 0) {
				g_filenames_enabled = false;
			} else {
				g_filenames_enabled = true;
			}
		}

		else {
			fprintf(stderr, "Error: Unrecognized argument %s\n", arg);
			print_usage();
			exit(1);
		}
	}

	return config;
}

#ifdef _MSC_VER
static rktest_enable_vterm_result_t enable_windows_virtual_terminal(void) {
	// Set output mode to handle virtual terminal sequences
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE) {
		return RKTEST_ENABLE_VTERM_ERROR_INVALID_HANDLE_VALUE;
	}

	DWORD dwMode = 0;
	if (!GetConsoleMode(hOut, &dwMode)) {
		return RKTEST_ENABLE_VTERM_ERROR_GET_CONSOLE_MODE_FAILED;
	}

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hOut, dwMode)) {
		return RKTEST_ENABLE_VTERM_ERROR_ENABLE_VIRTUAL_TERMINAL_FAILED;
	}

	return RKTEST_ENABLE_VTERM_OK;
}
#endif // WIN32

static rktest_config_t initialize(int argc, const char* argv[]) {
	rktest_config_t config = parse_args(argc, argv);

	g_colors_enabled = true;
	if (config.color_mode == RKTEST_COLOR_MODE_OFF) {
		g_colors_enabled = false;
	}

#ifdef _MSC_VER
	if (g_colors_enabled) {
		switch (enable_windows_virtual_terminal()) {
			case RKTEST_ENABLE_VTERM_ERROR_INVALID_HANDLE_VALUE:
				g_colors_enabled = false;
				fprintf(stderr, "Error: GetStdHandle returned INVALID_HANDLE_VALUE\n");
				break;
			case RKTEST_ENABLE_VTERM_ERROR_GET_CONSOLE_MODE_FAILED:
				g_colors_enabled = false;
				break;
			case RKTEST_ENABLE_VTERM_ERROR_ENABLE_VIRTUAL_TERMINAL_FAILED:
				g_colors_enabled = false;
				fprintf(stderr, "Error: SetConsoleMode to ENABLE_VIRTUAL_TERMINAL_PROCESSING failed\n");
				break;
			case RKTEST_ENABLE_VTERM_OK:
				break;
		}
	}
#endif // WIN32

	return config;
}

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
	*suite = (rktest_suite_t) { 0 };
	suite->name = suite_name;
	return suite;
}

static rktest_suite_t* find_or_add_suite(rktest_environment_t* env, const char* suite_name) {
	rktest_suite_t* suite = find_suite_with_name(env->test_suites, env->num_test_suites, suite_name);
	suite = suite ? suite : add_new_suite(env, suite_name);
	return suite;
}

static bool test_matches_filter(const rktest_test_t* test, const char* pattern) {
	if (*pattern == '\0') {
		return true;
	}

	char full_test_name[128];
	snprintf(full_test_name, sizeof(full_test_name) / sizeof(char), "%s.%s", test->suite_name, test->test_name);
	return string_wildcard_match(full_test_name, pattern);
}

// Loop through the entirety of the `rkdata` memory section, including padding.
// If the iterator `it` points to null, it's padding and we skip it.
// If it's non-null, we have a test and push it into `tests`.
static rktest_environment_t* setup_test_env(const rktest_config_t* config) {
	rktest_environment_t* env = malloc(sizeof(rktest_environment_t));
	*env = (rktest_environment_t) { 0 };

	for (const rktest_test_t* const* it = TEST_DATA_BEGIN; it != TEST_DATA_END; it++) {
		if (*it == NULL) {
			continue;
		}

		const rktest_test_t* const test = *it;

		if (env->num_test_suites == RKTEST_MAX_NUM_TEST_SUITES) {
			fprintf(stderr, "Error: number of test suites is greater than RKTEST_MAX_NUM_TEST_SUITES (%zu). "
							"See the `Config variables` section of rktest.h\n",
					RKTEST_MAX_NUM_TEST_SUITES);
			exit(1);
		}

		rktest_suite_t* suite = find_or_add_suite(env, test->suite_name);

		if (suite->total_num_tests == RKTEST_MAX_NUM_TESTS_PER_SUITE) {
			fprintf(stderr, "Error: number of tests in suite \"%s\" is greater than RKTEST_MAX_NUM_TESTS_PER_SUITE (%zu). "
							"See the `Config variables` section of rktest.h\n",
					suite->name,
					RKTEST_MAX_NUM_TESTS_PER_SUITE);
			exit(1);
		}

		/* Add test to suite */
		if (test_matches_filter(test, config->test_filter)) {
			if (string_starts_with(test->test_name, "DISABLED_")) {
				suite->test_is_disabled[suite->total_num_tests] = true;
				suite->num_disabled_tests++;
				env->total_num_disabled_tests++;
			} else {
				suite->test_is_disabled[suite->total_num_tests] = false;
				env->total_num_filtered_tests++;
			}

			/* Add test to suite */
			suite->tests[suite->total_num_tests] = *test;
			suite->total_num_tests++;
		}
	}

	/* Count number of suites actually containing tests*/
	foreach (rktest_suite_t*, suite, env->test_suites, env->num_test_suites) {
		if (suite->num_disabled_tests < suite->total_num_tests) {
			env->total_num_filtered_suites++;
		}
	}

	// return env;
	return env;
}

static bool run_test(const rktest_test_t* test, const rktest_config_t* config) {
	rktest_log_info("[ RUN      ] ", "%s.%s \n", test->suite_name, test->test_name);

	rktest_timer_t test_timer = rktest_timer_start();
	test->run();
	rktest_millis_t test_time_ms = rktest_timer_stop(&test_timer);

	const bool test_passed = !g_current_test_failed;
	g_current_test_failed = false;

	if (test_passed) {
		rktest_printf_green("[       OK ] ");
	} else {
		rktest_printf_red("[  FAILED  ] ");
	}
	printf("%s.%s ", test->suite_name, test->test_name);
	if (config->print_timestamps_enabled) {
		printf("(%d ms)", test_time_ms);
	}
	printf("\n");

	return test_passed;
}

static rktest_report_t* run_all_tests(rktest_environment_t* env, const rktest_config_t* config) {
	rktest_report_t* report = malloc(sizeof(rktest_report_t));
	*report = (rktest_report_t) {
		.failed_tests = { 0 },
		.num_failed_tests = 0,
		.num_passed_tests = 0,
	};

	foreach (rktest_suite_t*, suite, env->test_suites, env->num_test_suites) {
		/* Skip suite if all cases filtered out */
		if (suite->num_disabled_tests == suite->total_num_tests) {
			continue;
		}

		const size_t num_filtered_tests = suite->total_num_tests - suite->num_disabled_tests;
		rktest_log_info("[----------] ", "%zu tests from %s\n", num_filtered_tests, suite->name);
		rktest_timer_t suite_timer = rktest_timer_start();
		for (size_t i = 0; i < suite->total_num_tests; i++) {
			const rktest_test_t* test = &suite->tests[i];

			/* Check if test is disabled, skip it*/
			if (suite->test_is_disabled[i]) {
				rktest_log_warning("[ DISABLED ] ", "%s.%s\n", test->suite_name, test->test_name);
				continue;
			}

			/* Run non-disabled test */
			const bool test_passed = run_test(test, config);
			if (test_passed) {
				report->num_passed_tests++;
			} else {
				report->failed_tests[report->num_failed_tests] = *test;
				report->num_failed_tests++;
			}
		}
		rktest_millis_t suite_time_ms = rktest_timer_stop(&suite_timer);
		rktest_log_info("[----------] ", "%zu tests from %s ", num_filtered_tests, suite->name);
		if (config->print_timestamps_enabled) {
			printf("(%d ms total)", suite_time_ms);
		}
		printf("\n\n");
	}

	return report;
}

static void print_failed_tests(rktest_report_t* report) {
	rktest_log_error("[  FAILED  ] ", "%zu tests, listed below:\n", report->num_failed_tests);
	foreach (const rktest_test_t*, failed_test, report->failed_tests, report->num_failed_tests) {
		rktest_log_error("[  FAILED  ] ", "%s.%s\n", failed_test->suite_name, failed_test->test_name);
	}
	printf("\n");
	printf(" %zu FAILED TEST%s\n", report->num_failed_tests, report->num_failed_tests > 1 ? "S" : "");
}

int rktest_main(int argc, const char* argv[]) {
	rktest_config_t config = initialize(argc, argv);
	rktest_environment_t* env = setup_test_env(&config);

	if (*config.test_filter) {
		rktest_printf_yellow("Note: Test filter = %s\n", config.test_filter);
	}

	rktest_log_info("[==========] ", "Running %zu tests from %zu test suites.\n", env->total_num_filtered_tests, env->total_num_filtered_suites);
	rktest_log_info("[----------] ", "Global test environment set-up.\n");

	rktest_timer_t total_time_timer = rktest_timer_start();
	rktest_report_t* report = run_all_tests(env, &config);
	rktest_millis_t total_time_ms = rktest_timer_stop(&total_time_timer);

	rktest_log_info("[----------] ", "Global test environment tear-down.\n");
	rktest_log_info("[==========] ", "%zu tests from %zu test suites ran. ", env->total_num_filtered_tests, env->total_num_filtered_suites);
	if (config.print_timestamps_enabled) {
		printf("(%d ms total)", total_time_ms);
	}
	printf("\n");
	rktest_log_info("[  PASSED  ] ", "%zu tests.\n", report->num_passed_tests);

	const bool tests_failed = report->num_failed_tests > 0;
	if (tests_failed) {
		print_failed_tests(report);
	}

	if (env->total_num_disabled_tests > 0) {
		if (!tests_failed) {
			printf("\n");
		}
		rktest_printf_yellow("  YOU HAVE %zu DISABLED TEST%s\n", env->total_num_disabled_tests, env->total_num_disabled_tests > 1 ? "S" : "");
	}

	free(report);
	free(env);

	return tests_failed;
}
