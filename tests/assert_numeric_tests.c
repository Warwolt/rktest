#include "rktest/rktest.h"

#include <stdint.h>

// defining variables here to easily control if tests pass or fail.
static const int int_sum1 = 1 + 2;
static const int int_sum2 = 3 + 4;
static const long long_sum1 = 1 + 2;
static const long long_sum2 = 3 + 4;

/* Predicate checks */
TEST(assert_int, expect_true) {
	ASSERT_TRUE(int_sum1 == 3);
	ASSERT_TRUE_INFO(int_sum2 == 7, "int_sum2 = %d", int_sum2);
}

TEST(assert_int, expect_false) {
	ASSERT_FALSE(int_sum1 == int_sum2);
	ASSERT_FALSE_INFO(int_sum1 == int_sum2, "int_sum2 = %d", int_sum2);
}

/* Equality checks */
TEST(assert_int, expect_equal) {
	ASSERT_EQ(int_sum1, 3);
	ASSERT_LONG_EQ(long_sum1, 3);

	ASSERT_EQ_INFO(int_sum2, 7, "int_sum2 = %d\n", int_sum2);
	ASSERT_LONG_EQ_INFO(long_sum2, 7, "long_sum2 = %ld\n", long_sum2);
}

TEST(assert_int, expect_not_equal) {
	ASSERT_NE(int_sum1, int_sum2);
	ASSERT_LONG_NE(long_sum1, long_sum2);

	ASSERT_NE_INFO(int_sum1, int_sum2, "int_sum2 = %d\n", int_sum2);
	ASSERT_LONG_NE_INFO(long_sum1, long_sum2, "long_sum2 = %ld\n", int_sum2);
}

/* Comparison checks */
TEST(assert_int, expect_less_than) {
	ASSERT_LT(int_sum1, int_sum2);
	ASSERT_LONG_LT(long_sum1, long_sum2);

	ASSERT_LT_INFO(int_sum1, int_sum2, "int_sum2 = %d\n", int_sum2);
	ASSERT_LONG_LT_INFO(long_sum1, long_sum2, "long_sum2 = %ld\n", long_sum2);
}

TEST(assert_int, expect_less_than_equal) {
	ASSERT_LE(int_sum1, int_sum2);
	ASSERT_LONG_LE(long_sum1, long_sum2);

	ASSERT_LE_INFO(int_sum1, int_sum2, "int_sum2 = %d\n", int_sum2);
	ASSERT_LONG_LE_INFO(long_sum1, long_sum2, "long_sum2 = %d\n", long_sum2);
}

TEST(assert_int, expect_greater_than) {
	ASSERT_GT(int_sum2, int_sum1);
	ASSERT_LONG_GT(long_sum2, long_sum1);

	ASSERT_GT_INFO(int_sum2, int_sum1, "int_sum2 = %d\n", int_sum2);
	ASSERT_LONG_GT_INFO(long_sum2, long_sum1, "long_sum2 = %d\n", long_sum2);
}

TEST(assert_int, expect_greater_than_equal) {
	ASSERT_GE(int_sum2, int_sum1);
	ASSERT_LONG_GE(long_sum2, long_sum1);

	ASSERT_GE_INFO(int_sum2, int_sum1, "int_sum2 = %d\n", int_sum2);
	ASSERT_LONG_GE_INFO(long_sum2, long_sum1, "long_sum2 = %d\n", long_sum2);
}
