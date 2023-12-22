#include <rktest/rktest.h>

static int g_number = 0;

TEST_SETUP(fixture_tests) {
	printf("Test Setup\n");
	g_number = 0;
}

TEST_TEARDOWN(fixture_tests) {
	printf("Test TearDown\n");
}

TEST(fixture_tests, increment_number) {
	g_number += 1;
	EXPECT_EQ(g_number, 1);
}

TEST(fixture_tests, increment_number_again) {
	g_number += 1;
	EXPECT_EQ(g_number, 1);
}
