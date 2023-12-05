#include <rktest/rktest.h>

const float float_sum1 = 0.3f + 0.3f + 0.3f;

TEST(float_tests, hello) {
	ASSERT_FLOAT_EQ(float_sum1, 0.9f);
	ASSERT_FLOAT_EQ_INFO(float_sum1, 0.9f, "float_sum1 = %f", float_sum1);

	EXPECT_FLOAT_EQ(float_sum1, 0.9f);
	EXPECT_FLOAT_EQ_INFO(float_sum1, 0.9f, "float_sum1 = %f", float_sum1);
}
