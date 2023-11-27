#include "rktest/rktest.h"

#include <stdio.h>
#ifdef WIN32
#include <windows.h>
#endif

// Unit test case storage
//
// An `rktest` data section is added to the binary, and is used to store meta data about all unit
// test cases to be run. Three fragments are added: `begin`, `data`, and `end`. The names utilize
// that fragments are sorted and stored alphabetically in the binary.
//
// All unit test files adds local test data to the `rktest$data` fragment of the section with the
// TEST() macro, and the linker will then collect them into a single `rktest$data` fragment in the
// binary.
//
// This trick is based on the following article by Raymond Chen:
// "Using linker segments and __declspec(allocate(…)) to arrange data in a specific order"
// https://devblogs.microsoft.com/oldnewthing/20181107-00/?p=100155
//
// Example layout:
// +--------------+-----------------+-----------+
// | rktest$begin | init_data_begin | rktest.o |
// +--------------+-----------------+----------+
// |              | test_1_case_1   | test1.o  |
// | rktest$data  | test_1_case_2   | test1.o  |
// |              | test_2_case_1   | test2.o  |
// +--------------+-----------------+----------+
// | rktest$end   | init_data_end   | rktest.o |
// +--------------+-----------------+----------+
#pragma section("rktest$begin", read)
#pragma section("rktest$data", read)
#pragma section("rktest$end", read)

// Add `_rktest_test_t` pointers to mark the begining and the end of the `rktest` section.
// Test cases are added to `rktest$data` using the TEST() macro.
__declspec(allocate("rktest$begin")) const _rktest_test_t* const init_data_begin = NULL;
__declspec(allocate("rktest$end")) const _rktest_test_t* const init_data_end = NULL;

static bool g_colors_enabled = false;

bool _rktest_colors_enabled(void) {
	return g_colors_enabled;
}

#ifdef WIN32
static _rktest_result_t enable_windows_virtual_terminal(void) {
	// Set output mode to handle virtual terminal sequences
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE) {
		return _RKTEST_RESULT_ERROR;
	}

	DWORD dwMode = 0;
	if (!GetConsoleMode(hOut, &dwMode)) {
		return _RKTEST_RESULT_ERROR;
	}

	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hOut, dwMode)) {
		return _RKTEST_RESULT_ERROR;
	}

	return _RKTEST_RESULT_OK;
}
#endif // WIN32

static void init_test_runner(void) {
	g_colors_enabled = true;
#ifdef WIN32
	if (enable_windows_virtual_terminal() != _RKTEST_RESULT_OK) {
		fprintf(stderr, "Error: could not initialize color output\n");
		g_colors_enabled = false;
	}
#endif // WIN32
}

static void run_test(const _rktest_test_t* test) {
	_rktest_printf_green("[ RUN      ] ");
	printf("%s.%s \n", test->suite, test->name);
	if (test->func) {
		test->func();
	}
	_rktest_printf_green("[       OK ] ");
	printf("%s.%s \n", test->suite, test->name);
}

static int run_all_tests() {
	int num_tests = 0;
	for (const _rktest_test_t* const* it = (&init_data_begin + 1); it != &init_data_end; it++) {
		if (*it) {
			num_tests++;
			run_test(*it);
		}
	}
	return num_tests;
}

int rktest_main(void) {
	init_test_runner();

	_rktest_printf_green("[==========] ");
	printf("Running tests.\n");
	_rktest_printf_green("[----------] ");
	printf("Global test environment set-up.\n");

	const int num_tests = run_all_tests();

	printf("\n");
	_rktest_printf_green("[----------] ");
	printf("Global test environment tear-down.\n");
	_rktest_printf_green("[==========] ");
	printf("%d tests from 1 test suite ran.\n", num_tests);

	return 0;
}
