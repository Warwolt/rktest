#include <rktest/rktest.h>

#include <stdint.h>

#ifndef RKTEST_FAILING_TESTS
static const char a = 'a';
static const char b = 'b';
#else
static const char a = 'd';
static const char b = 'c';
#endif

TEST(char_tests, expect_equal) {
	ASSERT_CHAR_EQ(a, 'a');
	EXPECT_CHAR_EQ(b, 'b');
}
