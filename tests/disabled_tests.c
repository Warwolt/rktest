#include <rktest/rktest.h>

TEST(disabled_tests, DISABLED_this_test_should_not_run) {
	EXPECT_EQ(1 + 1, 3);
}

TEST(disabled_tests, this_test_should_run) {
	EXPECT_EQ(1 + 1, 2);
}
