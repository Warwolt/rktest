#include "sample1_factorial.h"

#include "rktest/rktest.h"
#include <limits.h>

TEST(factorial_test, factorial_of_negative_numbers_is_one) {
	EXPECT_EQ(factorial(-5), 1);
	EXPECT_EQ(factorial(-1), 1);
	EXPECT_GT(factorial(-10), 0);
}

TEST(factorial_test, factorial_of_zero_is_one) {
	EXPECT_EQ(factorial(0), 1);
}

TEST(factorial_test, factorial_of_positive_numbers) {
	EXPECT_EQ(factorial(1), 1);
	EXPECT_EQ(factorial(2), 2);
	EXPECT_EQ(factorial(3), 6);
	EXPECT_EQ(factorial(8), 40320);
}

TEST(is_prime_test, negative_numbers_are_not_prime) {
	EXPECT_FALSE(is_prime(-1));
	EXPECT_FALSE(is_prime(-2));
	EXPECT_FALSE(is_prime(INT_MIN));
}

TEST(is_prime_test, trivial_cases) {
	EXPECT_FALSE(is_prime(0));
	EXPECT_FALSE(is_prime(1));
	EXPECT_TRUE(is_prime(2));
	EXPECT_TRUE(is_prime(3));
}

TEST(is_prime_test, positive_prime_numbers) {
	EXPECT_FALSE(is_prime(4));
	EXPECT_TRUE(is_prime(5));
	EXPECT_FALSE(is_prime(6));
	EXPECT_TRUE(is_prime(23));
}
