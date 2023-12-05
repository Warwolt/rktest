#include "rktest/rktest.h"

const char* str1 = "hello";
const char* str2 = "world";

TEST(string_tests, strings_equal) {
	ASSERT_STREQ(str1, "hello");
	ASSERT_STREQ_INFO(str2, "world", "str2 = %s\n", str2);

	EXPECT_STREQ(str1, "hello");
	EXPECT_STREQ_INFO(str2, "world", "str2 = %s\n", str2);
}

TEST(string_tests, strings_not_equal) {
	ASSERT_STRNE(str1, "hello123");
	ASSERT_STRNE_INFO(str2, "world123", "str2 = %s\n", str2);

	EXPECT_STRNE(str1, "hello123");
	EXPECT_STRNE_INFO(str2, "world123", "str2 = %s\n", str2);
}

TEST(string_tests, strings_case_equal) {
	ASSERT_CASE_STREQ(str1, "Hello");
	ASSERT_CASE_STREQ_INFO(str2, "World", "str2 = %s\n", str2);

	EXPECT_CASE_STREQ(str1, "hello");
	EXPECT_CASE_STREQ_INFO(str2, "world", "str2 = %s\n", str2);
}
