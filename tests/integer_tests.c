#include <rktest/rktest.h>

#include <stdint.h>

#ifndef RKTEST_FAILING_TESTS
static const int int_sum1 = 1 + 2;
static const int int_sum2 = 3 + 4;
static const char char_sum1 = 1 + 2;
static const char char_sum2 = 3 + 4;
static const long long_sum1 = 1 + 2;
static const long long_sum2 = 3 + 4;
#else
static const int int_sum1 = 1 + 2 + 50;
static const int int_sum2 = 3 + 4 + 10;
static const char char_sum1 = 1 + 2 + 10;
static const char char_sum2 = 3 + 4 + 10;
static const long long_sum1 = 1 + 2 + 10;
static const long long_sum2 = 3 + 4 + 10;
#endif

/* Predicate checks */
TEST(integer_tests, expect_true) {
	ASSERT_TRUE(int_sum1 == 3);
	EXPECT_TRUE(int_sum1 == 3);
}

TEST(integer_tests, expect_true_info) {
	ASSERT_TRUE_INFO(int_sum2 == 7, "int_sum2 = %d\n", int_sum2);
	EXPECT_TRUE_INFO(int_sum2 == 7, "int_sum2 = %d\n", int_sum2);
}

TEST(integer_tests, expect_false) {
	ASSERT_FALSE(int_sum1 == 1 + 2 + 50);
	EXPECT_FALSE(int_sum1 == int_sum2);
}

TEST(integer_tests, expect_false_info) {
	ASSERT_FALSE_INFO(int_sum1 == int_sum2, "int_sum2 = %d", int_sum2);
	EXPECT_FALSE_INFO(int_sum1 == int_sum2, "int_sum2 = %d", int_sum2);
}

/* Equality checks */
TEST(integer_tests, expect_equal) {
	ASSERT_EQ(int_sum1, 3);
	ASSERT_EQ(char_sum1, 3);
	ASSERT_LONG_EQ(long_sum1, 3);

	EXPECT_EQ(int_sum1, 3);
	EXPECT_EQ(char_sum1, 3);
	EXPECT_LONG_EQ(long_sum1, 3);
}

TEST(integer_tests, expect_equal_info) {
	ASSERT_EQ_INFO(int_sum2, 7, "int_sum2 = %d\n", int_sum2);
	ASSERT_EQ_INFO(char_sum2, 7, "char_sum2 = %d\n", char_sum2);
	ASSERT_LONG_EQ_INFO(long_sum2, 7, "long_sum2 = %ld\n", long_sum2);

	EXPECT_EQ_INFO(int_sum2, 7, "int_sum2 = %d\n", int_sum2);
	EXPECT_EQ_INFO(char_sum2, 7, "char_sum2 = %d\n", char_sum2);
	EXPECT_LONG_EQ_INFO(long_sum2, 7, "long_sum2 = %ld\n", long_sum2);
}

TEST(integer_tests, expect_not_equal) {
	ASSERT_NE(int_sum1, int_sum2);
	ASSERT_NE(char_sum1, char_sum2);
	ASSERT_LONG_NE(long_sum1, long_sum2);

	EXPECT_NE(int_sum1, int_sum2);
	EXPECT_NE(char_sum1, char_sum2);
	EXPECT_LONG_NE(long_sum1, long_sum2);
}

TEST(integer_tests, expect_not_equal_info) {
	ASSERT_NE(int_sum1, int_sum2);
	ASSERT_NE(char_sum1, char_sum2);
	ASSERT_LONG_NE(long_sum1, long_sum2);

	EXPECT_NE(int_sum1, int_sum2);
	EXPECT_NE(char_sum1, char_sum2);
	EXPECT_LONG_NE(long_sum1, long_sum2);
}

/* Comparison checks */
TEST(integer_tests, expect_less_than) {
	ASSERT_LT(int_sum1, int_sum2);
	ASSERT_LT(char_sum1, char_sum2);
	ASSERT_LONG_LT(long_sum1, long_sum2);

	EXPECT_LT(int_sum1, int_sum2);
	EXPECT_LT(char_sum1, char_sum2);
	EXPECT_LONG_LT(long_sum1, long_sum2);
}

TEST(integer_tests, expect_less_than_info) {
	ASSERT_LT_INFO(int_sum1, int_sum2, "int_sum2 = %d\n", int_sum2);
	ASSERT_LT_INFO(char_sum1, char_sum2, "char_sum2 = %d\n", char_sum2);
	ASSERT_LONG_LT_INFO(long_sum1, long_sum2, "long_sum2 = %ld\n", long_sum2);

	EXPECT_LT_INFO(int_sum1, int_sum2, "int_sum2 = %d\n", int_sum2);
	EXPECT_LT_INFO(char_sum1, char_sum2, "char_sum2 = %d\n", char_sum2);
	EXPECT_LONG_LT_INFO(long_sum1, long_sum2, "long_sum2 = %ld\n", long_sum2);
}

TEST(integer_tests, expect_less_than_equal) {
	ASSERT_LE(int_sum1, int_sum2);
	ASSERT_LE(char_sum1, char_sum2);
	ASSERT_LONG_LE(long_sum1, long_sum2);

	EXPECT_LE(int_sum1, int_sum2);
	EXPECT_LE(char_sum1, char_sum2);
	EXPECT_LONG_LE(long_sum1, long_sum2);
}

TEST(integer_tests, expect_less_than_equal_info) {
	ASSERT_LE_INFO(int_sum1, int_sum2, "int_sum2 = %d\n", int_sum2);
	ASSERT_LE_INFO(char_sum1, char_sum2, "char_sum2 = %d\n", char_sum2);
	ASSERT_LONG_LE_INFO(long_sum1, long_sum2, "long_sum2 = %ld\n", long_sum2);

	EXPECT_LE_INFO(int_sum1, int_sum2, "int_sum2 = %d\n", int_sum2);
	EXPECT_LE_INFO(char_sum1, char_sum2, "char_sum2 = %d\n", char_sum2);
	EXPECT_LONG_LE_INFO(long_sum1, long_sum2, "long_sum2 = %ld\n", long_sum2);
}

TEST(integer_tests, expect_greater_than) {
	ASSERT_GT(int_sum2, int_sum1);
	ASSERT_GT(char_sum2, char_sum1);
	ASSERT_LONG_GT(long_sum2, long_sum1);

	EXPECT_GT(int_sum2, int_sum1);
	EXPECT_GT(char_sum2, char_sum1);
	EXPECT_LONG_GT(long_sum2, long_sum1);
}

TEST(integer_tests, expect_greater_than_info) {
	ASSERT_GT_INFO(int_sum2, int_sum1, "int_sum2 = %d\n", int_sum2);
	ASSERT_GT_INFO(char_sum2, char_sum1, "char_sum2 = %d\n", char_sum2);
	ASSERT_LONG_GT_INFO(long_sum2, long_sum1, "long_sum2 = %ld\n", long_sum2);

	EXPECT_GT_INFO(int_sum2, int_sum1, "int_sum2 = %d\n", int_sum2);
	EXPECT_GT_INFO(char_sum2, char_sum1, "char_sum2 = %d\n", char_sum2);
	EXPECT_LONG_GT_INFO(long_sum2, long_sum1, "long_sum2 = %ld\n", long_sum2);
}

TEST(integer_tests, expect_greater_than_equal) {
	ASSERT_GE(int_sum2, int_sum1);
	ASSERT_GE(char_sum2, char_sum1);
	ASSERT_LONG_GE(long_sum2, long_sum1);

	EXPECT_GE(int_sum2, int_sum1);
	EXPECT_GE(char_sum2, char_sum1);
	EXPECT_LONG_GE(long_sum2, long_sum1);
}

TEST(integer_tests, expect_greater_than_equal_info) {
	ASSERT_GE_INFO(int_sum2, int_sum1, "int_sum2 = %d\n", int_sum2);
	ASSERT_GE_INFO(char_sum2, char_sum1, "char_sum2 = %d\n", char_sum2);
	ASSERT_LONG_GE_INFO(long_sum2, long_sum1, "long_sum2 = %ld\n", long_sum2);

	EXPECT_GE_INFO(int_sum2, int_sum1, "int_sum2 = %d\n", int_sum2);
	EXPECT_GE_INFO(char_sum2, char_sum1, "char_sum2 = %d\n", char_sum2);
	EXPECT_LONG_GE_INFO(long_sum2, long_sum1, "long_sum2 = %ld\n", long_sum2);
}
