#include "rktest/rktest.h"

#include <memory.h>
#include <stdio.h>
#include <string.h>

#ifdef WIN32
#include <windows.h>
#endif

/* RK Test runner internals ------------------------------------------------- */
#define RKTEST_MAX_NUM_TESTS (RKTEST_MAX_NUM_TEST_SUITES * RKTEST_MAX_NUM_TESTS_PER_SUITE)

#define foreach(type_ptr, iter, array, array_len) \
	for (type_ptr iter = &array[0]; iter != &array[array_len]; iter++)

typedef enum {
	RKTEST_RESULT_OK,
	RKTEST_RESULT_ERROR,
} rktest_result_t;

typedef struct {
	const char* name;
	void (*func)(void);
} rktest_test_t;

typedef struct {
	const char* name;
	rktest_test_t tests[RKTEST_MAX_NUM_TESTS_PER_SUITE];
	size_t num_tests;
} rktest_suite_t;

// Unit test case storage
//
// An `rktest` data section is added to the binary, and is used to store meta
// data about all unit test cases to be run. Three fragments are added: `begin`,
// `data`, and `end`. The names utilize that fragments are sorted and stored
// alphabetically in the binary.
//
// All unit test files adds local test data to the `rktest$data` fragment of the
// section with the TEST() macro, and the linker will then collect them into a
// single `rktest$data` fragment in the binary.
//
// This trick is based on the following article by Raymond Chen: "Using linker
// segments and __declspec(allocate(…)) to arrange data in a specific order"
// https://devblogs.microsoft.com/oldnewthing/20181107-00/?p=100155
//
// Example layout:
// +--------------+-----------------+-----------+
// | rktest$begin | test_data_begin | rktest.o |
// +--------------+-----------------+----------+
// |              | test_1_case_1   | test1.o  |
// | rktest$data  | test_1_case_2   | test1.o  |
// |              | test_2_case_1   | test2.o  |
// +--------------+-----------------+----------+
// | rktest$end   | test_data_end   | rktest.o |
// +--------------+-----------------+----------+
#pragma section("rktest$begin", read)
#pragma section("rktest$data", read)
#pragma section("rktest$end", read)

// Add `rktest_macro_data_t` pointers to mark the begining and the end of the
// `rktest` memory section. Test cases are added to `rktest$data` using the
// TEST() macro.
__declspec(allocate("rktest$begin")) const rktest_macro_data_t* const test_data_begin = NULL;
__declspec(allocate("rktest$end")) const rktest_macro_data_t* const test_data_end = NULL;

static bool g_colors_enabled = false;

bool rktest_colors_enabled(void) {
	return g_colors_enabled;
}

#ifdef WIN32
static rktest_result_t enable_windows_virtual_terminal(void) {
	// Set output mode to handle virtual terminal sequences
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE) {
		return RKTEST_RESULT_ERROR;
	}

	DWORD dwMode = 0;
	if (!GetConsoleMode(hOut, &dwMode)) {
		return RKTEST_RESULT_ERROR;
	}

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hOut, dwMode)) {
		return RKTEST_RESULT_ERROR;
	}

	return RKTEST_RESULT_OK;
}
#endif // WIN32

static void initialize(void) {
	g_colors_enabled = true;
#ifdef WIN32
	if (enable_windows_virtual_terminal() != RKTEST_RESULT_OK) {
		fprintf(stderr, "Error: could not initialize color output\n");
		g_colors_enabled = false;
	}
#endif // WIN32
}

static rktest_suite_t* find_suite(rktest_suite_t* suites, size_t num_suites, const char* suite_name) {
	foreach (rktest_suite_t*, suite, suites, num_suites) {
		if (strcmp(suite->name, suite_name) == 0) {
			return suite;
		}
	}
	return NULL;
}

// Loop through the entirety of the `rkdata` memory section, including padding.
// If the iterator `it` points to null, it's padding and we skip it.
// If it's non-null, we have a test and push it into `tests`.
static size_t register_tests(rktest_suite_t* suites) {
	size_t num_suites = 0;
	for (const rktest_macro_data_t* const* it = (&test_data_begin + 1); it != &test_data_end; it++) {
		if (!*it) {
			continue;
		}

		if (num_suites == RKTEST_MAX_NUM_TEST_SUITES) {
			fprintf(stderr, "Error: number of test suites is greater than RKTEST_MAX_NUM_TEST_SUITES (%zu)."
							"See the `Config variables` section of rktest.h\n",
					RKTEST_MAX_NUM_TEST_SUITES);
			exit(1);
		}

		const rktest_macro_data_t* const data = *it;
		rktest_suite_t* suite = find_suite(suites, num_suites, data->suite_name);
		if (!suite) {
			// add new suite
			suite = &suites[num_suites++];
			*suite = (rktest_suite_t) {
				.name = data->suite_name,
				.num_tests = 0,
				.tests = { 0 },
			};
		}

		if (suite->num_tests == RKTEST_MAX_NUM_TESTS_PER_SUITE) {
			fprintf(stderr, "Error: number of tests in suite %s is greater than RKTEST_MAX_NUM_TESTS_PER_SUITE (%zu)."
							"See the `Config variables` section of rktest.h\n",
					suite->name,
					RKTEST_MAX_NUM_TESTS_PER_SUITE);
			exit(1);
		}

		// add test to suite
		rktest_test_t* test = &suite->tests[suite->num_tests++];
		*test = (rktest_test_t) {
			.name = data->test_name,
			.func = data->func,
		};
	}
	return num_suites;
}

static void run_test(const rktest_test_t* test, const char* suite_name) {
	rktest_printf_test_step("[ RUN      ] ", "%s.%s \n", suite_name, test->name);
	if (test->func) {
		test->func();
	}
	rktest_printf_test_step("[       OK ] ", "%s.%s \n", suite_name, test->name);
}

static size_t run_all_tests(rktest_suite_t* suites, size_t num_suites) {
	size_t num_tests = 0;
	foreach (rktest_suite_t*, suite, suites, num_suites) {
		foreach (rktest_test_t*, test, suite->tests, suite->num_tests) {
			run_test(test, suite->name);
			num_tests++;
		}
	}
	return num_tests;
}

int rktest_main(void) {
	initialize();

	rktest_printf_test_step("[==========] ", "Running tests.\n");

	rktest_suite_t* test_suites = malloc(RKTEST_MAX_NUM_TEST_SUITES * sizeof(rktest_suite_t));
	if (!test_suites) {
		fprintf(stderr, "Error: could not allocate space for test data");
	}

	const size_t num_test_suites = register_tests(test_suites);

	rktest_printf_test_step("[----------] ", "Global test environment set-up.\n");

	const size_t num_tests = run_all_tests(test_suites, num_test_suites);

	printf("\n");
	rktest_printf_test_step("[----------] ", "Global test environment tear-down.\n");
	rktest_printf_test_step("[==========] ", "%zu tests from %zu test suite ran.\n", num_tests, num_test_suites);

	free(test_suites);
	return 0;
}
