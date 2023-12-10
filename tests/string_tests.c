#include <rktest/rktest.h>

#ifndef RKTEST_FAILING_TESTS
const char* str1 = "hello";
const char* str2 = "world";
#else
const char* str1 = "elloh";
const char* str2 = "dlorw";
#endif

TEST(string_tests, strings_equal) {
	ASSERT_STREQ(str1, "hello");
	EXPECT_STREQ(str1, "hello");
}

TEST(string_tests, strings_equal_info) {
	ASSERT_STREQ_INFO(str2, "world", "str2 = %s\n", str2);
	EXPECT_STREQ_INFO(str2, "world", "str2 = %s\n", str2);
}

TEST(string_tests, strings_not_equal) {
	ASSERT_STRNE(str1, "hello123");
	EXPECT_STRNE(str1, "hello123");
}

TEST(string_tests, strings_not_equal_info) {
	ASSERT_STRNE_INFO(str2, "world123", "str2 = %s\n", str2);
	EXPECT_STRNE_INFO(str2, "world123", "str2 = %s\n", str2);
}

TEST(string_tests, strings_case_equal) {
	ASSERT_CASE_STREQ(str1, "Hello");
	EXPECT_CASE_STREQ(str1, "hello");
}

TEST(string_tests, strings_case_equal_info) {
	ASSERT_CASE_STREQ_INFO(str2, "World", "str2 = %s\n", str2);
	EXPECT_CASE_STREQ_INFO(str2, "world", "str2 = %s\n", str2);
}

TEST(string_tests, strings_case_not_equal) {
	ASSERT_CASE_STRNE(str1, "Hello123");
	EXPECT_CASE_STRNE(str1, "hello123");
}

TEST(string_tests, strings_case_not_equal_info) {
	ASSERT_CASE_STRNE_INFO(str2, "World123", "str2 = %s\n", str2);
	EXPECT_CASE_STRNE_INFO(str2, "world123", "str2 = %s\n", str2);
}
