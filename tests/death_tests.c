#include <rktest/rktest.h>

#include <stdio.h>
#include <stdlib.h>

#ifndef RKTEST_FAILING_TESTS
static const int exit_code = 1;
static const char* expected_stderr1 = "Error!";
static const char* expected_stderr2 = "Error?";
#else
static const int exit_code = 0;
static const char* expected_stderr1 = "Error!!";
static const char* expected_stderr2 = "Error??";
#endif

TEST(death_tests, expect_death) {
	ASSERT_DEATH({
		fprintf(stderr, "Error!\n");
		exit(exit_code);
	}, expected_stderr1);
	EXPECT_DEATH({
		fprintf(stderr, "Error?\n");
		exit(exit_code);
	}, expected_stderr2);
}
