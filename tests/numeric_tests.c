#include "rktest/rktest.h"

#include <stdint.h>

// defining variables here to easily control if tests pass or fail.
static const int int_sum1 = 1 + 2;
static const int int_sum2 = 3 + 4;
static const char char_sum1 = 1 + 2;
static const char char_sum2 = 3 + 4;
static const long long_sum1 = 1 + 2;
static const long long_sum2 = 3 + 4;

/* Predicate checks */
TEST(numeric_tests, expect_true) {
	EXPECT_TRUE(int_sum1 == 3);
	EXPECT_TRUE_INFO(int_sum2 == 7, "int_sum2 = %d\n", int_sum2);
}

TEST(numeric_tests, expect_false) {
	EXPECT_FALSE(int_sum1 == int_sum2);
	EXPECT_FALSE_INFO(int_sum1 == int_sum2, "int_sum2 = %d", int_sum2);
}

/* Equality checks */
TEST(numeric_tests, expect_equal) {
	EXPECT_EQ(int_sum1, 3);
	EXPECT_EQ(char_sum1, 3);
	EXPECT_LONG_EQ(long_sum1, 3);

	EXPECT_EQ_INFO(int_sum2, 7, "int_sum2 = %d\n", int_sum2);
	EXPECT_EQ_INFO(char_sum2, 7, "char_sum2 = %d\n", char_sum2);
	EXPECT_LONG_EQ_INFO(long_sum2, 7, "long_sum2 = %ld\n", long_sum2);
}

TEST(numeric_tests, expect_not_equal) {
	EXPECT_NE(int_sum1, int_sum2);
	EXPECT_NE(char_sum1, char_sum2);
	EXPECT_LONG_NE(long_sum1, long_sum2);

	EXPECT_NE_INFO(int_sum1, int_sum2, "int_sum2 = %d\n", int_sum2);
	EXPECT_NE_INFO(char_sum1, char_sum2, "char_sum2 = %d\n", char_sum2);
	EXPECT_LONG_NE_INFO(long_sum1, long_sum2, "long_sum2 = %ld\n", long_sum2);
}

/* Comparison checks */
TEST(numeric_tests, expect_less_than) {
	EXPECT_LT(int_sum1, int_sum2);
	EXPECT_LT(char_sum1, char_sum2);
	EXPECT_LONG_LT(long_sum1, long_sum2);

	EXPECT_LT_INFO(int_sum1, int_sum2, "int_sum2 = %d\n", int_sum2);
	EXPECT_LT_INFO(char_sum1, char_sum2, "char_sum2 = %d\n", char_sum2);
	EXPECT_LONG_LT_INFO(long_sum1, long_sum2, "long_sum2 = %ld\n", long_sum2);
}

TEST(numeric_tests, expect_less_than_equal) {
	EXPECT_LE(int_sum1, int_sum2);
	EXPECT_LE(char_sum1, char_sum2);
	EXPECT_LONG_LE(long_sum1, long_sum2);

	EXPECT_LE_INFO(int_sum1, int_sum2, "int_sum2 = %d\n", int_sum2);
	EXPECT_LE_INFO(char_sum1, char_sum2, "char_sum2 = %d\n", char_sum2);
	EXPECT_LONG_LE_INFO(long_sum1, long_sum2, "long_sum2 = %ld\n", long_sum2);
}

TEST(numeric_tests, expect_greater_than) {
	EXPECT_GT(int_sum2, int_sum1);
	EXPECT_GT(char_sum2, char_sum1);
	EXPECT_LONG_GT(long_sum2, long_sum1);

	EXPECT_GT_INFO(int_sum2, int_sum1, "int_sum2 = %d\n", int_sum2);
	EXPECT_GT_INFO(char_sum2, char_sum1, "char_sum2 = %d\n", char_sum2);
	EXPECT_LONG_GT_INFO(long_sum2, long_sum1, "long_sum2 = %ld\n", long_sum2);
}

TEST(numeric_tests, expect_greater_than_equal) {
	EXPECT_GE(int_sum2, int_sum1);
	EXPECT_GE(char_sum2, char_sum1);
	EXPECT_LONG_GE(long_sum2, long_sum1);

	EXPECT_GE_INFO(int_sum2, int_sum1, "int_sum2 = %d\n", int_sum2);
	EXPECT_GE_INFO(char_sum2, char_sum1, "char_sum2 = %d\n", char_sum2);
	EXPECT_LONG_GE_INFO(long_sum2, long_sum1, "long_sum2 = %ld\n", long_sum2);
}
