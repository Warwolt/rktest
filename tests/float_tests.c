#include <rktest/rktest.h>

#ifndef RKTEST_FAILING_TESTS
const float float_sum = 0.3f + 0.3f + 0.3f;
const double double_sum = 0.3 + 0.3 + 0.3;
#else
const float float_sum = 0.3f + 0.3f + 0.3f + 0.1f;
const double double_sum = 0.3 + 0.3 + 0.3 + 0.1;
#endif

TEST(float_tests, float_equal) {
	ASSERT_FLOAT_EQ(float_sum, 0.9f);
	EXPECT_FLOAT_EQ(float_sum, 0.9f);
}

TEST(float_tests, float_equal_info) {
	ASSERT_FLOAT_EQ_INFO(float_sum, 0.9f, "Hello world!\n");
	EXPECT_FLOAT_EQ_INFO(float_sum, 0.9f, "Hello world!\n");
}

TEST(float_tests, double_equal) {
	ASSERT_DOUBLE_EQ(double_sum, 0.9);
	EXPECT_DOUBLE_EQ(double_sum, 0.9);
}

TEST(float_tests, double_equal_info) {
	ASSERT_DOUBLE_EQ_INFO(double_sum, 0.9, "Hello world!\n");
	EXPECT_DOUBLE_EQ_INFO(double_sum, 0.9, "Hello world!\n");
}
