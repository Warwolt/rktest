#include "sample1_factorial.h"

#include "rktest/rktest.h"
#include <limits.h>

TEST(factorial_test, negative) {
	EXPECT_EQ(1, factorial(-5));
	EXPECT_EQ(1, factorial(-1));
	EXPECT_GT(factorial(-10), 0);
}

TEST(factorial_test, zero) {
	EXPECT_EQ(1, factorial(0));
}

TEST(factorial_test, positive) {
	EXPECT_EQ(1, factorial(1));
	EXPECT_EQ(2, factorial(2));
	EXPECT_EQ(6, factorial(3));
	EXPECT_EQ(40320, factorial(8));
}

TEST(is_prime_test, negative) {
	EXPECT_FALSE(is_prime(-1));
	EXPECT_FALSE(is_prime(-2));
	EXPECT_FALSE(is_prime(INT_MIN));
}

TEST(is_prime_test, trivial) {
	EXPECT_FALSE(is_prime(0));
	EXPECT_FALSE(is_prime(1));
	EXPECT_TRUE(is_prime(2));
	EXPECT_TRUE(is_prime(3));
}

TEST(is_prime_test, positive) {
	EXPECT_FALSE(is_prime(4));
	EXPECT_TRUE(is_prime(5));
	EXPECT_FALSE(is_prime(6));
	EXPECT_TRUE(is_prime(23));
}
