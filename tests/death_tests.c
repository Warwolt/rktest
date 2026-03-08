#include <rktest/rktest.h>

#include <stdio.h>
#include <stdlib.h>

#define RKTEST_CHECK_DEATH(expr, expected_stderr, is_assert)                        \
	if (rktest_death_test_line() == __LINE__) {                                     \
		expr;                                                                       \
	} else if (rktest_death_test_line() == 0) {                                     \
		bool did_pass = rktest_run_death_test(__FILE__, __LINE__, expected_stderr); \
		if (!did_pass && is_assert) {                                               \
			return;                                                                 \
		}                                                                           \
	}

#define EXPECT_DEATH(expr, expected_stderr) RKTEST_CHECK_DEATH(expr, expected_stderr, RKTEST_CHECK_EXPECT)
#define ASSERT_DEATH(expr, expected_stderr) RKTEST_CHECK_DEATH(expr, expected_stderr, RKTEST_CHECK_ASSERT)

TEST(death_tests, expect_death) {
	ASSERT_DEATH({
		fprintf(stderr, "Error!\n");
		exit(0);
	}, "Error!!");
	EXPECT_DEATH({
		fprintf(stderr, "Error?\n");
		exit(1);
	}, "Error??");
}
