#include "rktest.h"

#include <stdio.h>

// Unit test case storage
//
// A `rktestdata` data section is added to the binary, and is used to store meta data about all unit
// test cases to be ran. Three fragments are added `begin`, `data`, `end`, and utilize that
// fragments are sorted and stored alphabetically in the binary.
//
// All unit test files adds local test data to the `rktestdata$data` fragment of the section with the
// TEST() macro, and the linker will then collect them into a single `rktestdata$data` fragment in the
// binary.
//
// This trick is based on the following article by Raymond Chen:
// "Using linker segments and __declspec(allocate(…)) to arrange data in a specific order"
// https://devblogs.microsoft.com/oldnewthing/20181107-00/?p=100155
//
// Layout:
// +------------------+-----------------+-----------+
// | rktestdata$begin | init_data_begin | rktest.o |
// +------------------+-----------------+----------+
// |                  | test_1_case_1   | test1.o  |
// | rktestdata$data  | test_1_case_2   | test1.o  |
// |                  | test_2_case_1   | test2.o  |
// +------------------+-----------------+----------+
// | rktestdata$end   | init_data_end   | rktest.o |
// +------------------+-----------------+----------+
#pragma section("rktestdata$begin", read)
#pragma section("rktestdata$data", read)
#pragma section("rktestdata$end", read)

// Add `rktest_test_t` pointers to mark the begining and the end of the `rktestdata` section.
// Test cases are added to `rktestdata$data` using the TEST() macro.
__declspec(allocate("rktestdata$begin")) const rktest_test_t* const init_data_begin = NULL;
__declspec(allocate("rktestdata$end")) const rktest_test_t* const init_data_end = NULL;

static void run_test(const rktest_test_t* test) {
	printf("[%s.%s]\n", test->suite, test->name);
	if (test->func) {
		test->func();
	}
}

int rktest_main(void) {
	for (const rktest_test_t* const* it = (&init_data_begin + 1); it != &init_data_end; it++) {
		if (*it) {
			run_test(*it);
		}
	}

	return 0;
}
