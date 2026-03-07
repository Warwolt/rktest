#include <rktest/rktest.h>

#include <stdio.h>
#include <stdlib.h>

#define ASSERT_DEATH(expr, message)                \
	if (__LINE__ == rktest_death_test_line()) {    \
		printf("%s %d\n", __FUNCTION__, __LINE__); \
		expr;                                      \
	}

TEST(death_tests, foo) {
	ASSERT_DEATH({
        fprintf(stderr, "Error!\n");
		exit(1);
    }, "Error!");
}
