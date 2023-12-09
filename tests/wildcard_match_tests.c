#include <rktest/rktest.h>

static bool string_wildcard_match(const char* str, const char* pattern) {
	if (*str == '\0') {
		return *pattern == '\0';
	}

	return strcmp(str, pattern) == 0;
}

TEST(wildcard_match_tests, empty_pattern_matches_only_empty_string) {
	EXPECT_TRUE(string_wildcard_match("", ""));
	EXPECT_FALSE(string_wildcard_match("strawberry", ""));
}

TEST(wildcard_match_tests, literal_pattern_matches_only_exact_literal) {
	EXPECT_TRUE(string_wildcard_match("strawberry", "strawberry"));
	EXPECT_FALSE(string_wildcard_match("straw", "strawberry"));
	EXPECT_FALSE(string_wildcard_match("berry", "strawberry"));
}
