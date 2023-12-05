#include <rktest/rktest.h>

const float float_sum1 = 0.3f + 0.3f + 0.3f;

TEST(float_tests, hello) {
	EXPECT_FLOAT_EQ(float_sum1, 0.9f);
}
