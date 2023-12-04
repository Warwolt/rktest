#include "rktest/rktest.h"

const char* str1 = "hello";
const char* str2 = "world";

TEST(string_tests, strings_equal) {
	ASSERT_STREQ(str1, "hello");
	ASSERT_STREQ_INFO(str2, "world", "str2 = %s\n", str2);

	EXPECT_STREQ(str1, "hello");
	EXPECT_STREQ_INFO(str2, "world", "str2 = %s\n", str2);
}
