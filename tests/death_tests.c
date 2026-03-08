#include <rktest/rktest.h>

// TODO: implement death test support on Linux / MacOS
// snapshot tests have to run the same on all three platforms,
// so for now we keep these tests disabled.
//
// #define ENABLE_DEATH_TEST_TESTS

#if defined(ENABLE_DEATH_TEST_TESTS)

#include <stdio.h>
#include <stdlib.h>

#ifndef RKTEST_FAILING_TESTS
static const int exit_code = 1;
static const char* expected_stderr1 = "Hello World";
static const char* expected_stderr2 = "Foo Bar";
#else
static const int exit_code = 0;
static const char* expected_stderr1 = "Hello Moon";
static const char* expected_stderr2 = "Bar Baz";
#endif

TEST(death_tests, expect_death) {
	ASSERT_DEATH({
		fprintf(stderr, "Hello World\n");
		exit(exit_code);
	}, expected_stderr1);
	EXPECT_DEATH({
		fprintf(stderr, "Foo Bar\n");
		exit(exit_code);
	}, expected_stderr2);
}

#endif // defined(ENABLE_DEATH_TEST_TESTS)
