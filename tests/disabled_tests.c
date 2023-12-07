#include <rktest/rktest.h>

TEST(disabled_tests, DISABLED_this_test_should_not_run) {
	printf("Hello world\n");
}

TEST(disabled_tests, DISABLED_this_test_should_run) {
	EXPECT_EQ(1 + 1, 2);
}
