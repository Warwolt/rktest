#include <rktest/rktest.h>

const float float_sum = 0.3f + 0.3f + 0.3f;
const double double_sum = 0.3 + 0.3 + 0.3;

TEST(float_tests, float_equal) {
	ASSERT_FLOAT_EQ(float_sum, 0.9f);
	ASSERT_FLOAT_EQ_INFO(float_sum, 0.9f, "float_sum = %f\n", float_sum);

	EXPECT_FLOAT_EQ(float_sum, 0.9f);
	EXPECT_FLOAT_EQ_INFO(float_sum, 0.9f, "float_sum = %f\n", float_sum);
}

TEST(float_tests, double_equal) {
	ASSERT_DOUBLE_EQ(double_sum, 0.9);
	ASSERT_DOUBLE_EQ_INFO(double_sum, 0.9, "double_sum = %f\n", double_sum);

	EXPECT_DOUBLE_EQ(double_sum, 0.9);
	EXPECT_DOUBLE_EQ_INFO(double_sum, 0.9, "double_sum = %f\n", double_sum);
}
