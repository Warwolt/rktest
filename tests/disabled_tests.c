#include <rktest/rktest.h>

// This test suite should show up, but this test should be printed as [ DISABLED ]
TEST(disabled_tests, DISABLED_this_test_should_not_run) {
	EXPECT_EQ(1 + 1, 3);
}

TEST(disabled_tests, this_test_should_run) {
	EXPECT_EQ(1 + 1, 2);
}

// This suite test should NOT show up since all its tests are disabled
TEST(disabled_tests2, DISABLED_this_test_should_not_run) {
	EXPECT_EQ(1 + 1, 3);
}

TEST(disabled_tests2, DISABLED_this_test_should_not_run_either) {
	EXPECT_EQ(1 + 1, 5);
}
