#include <rktest/rktest.h>

static bool string_wildcard_match(const char* str, const char* pattern) {
	if (pattern[0] == '*' && pattern[1] == '\0') {
		return true;
	}

	return strcmp(str, pattern) == 0;
}

TEST(wildcard_match_tests, empty_pattern_matches_only_empty_string) {
	EXPECT_TRUE(string_wildcard_match("", ""));
	EXPECT_FALSE(string_wildcard_match("strawberry", ""));
	EXPECT_FALSE(string_wildcard_match("straw", ""));
	EXPECT_FALSE(string_wildcard_match("berry", ""));
}

TEST(wildcard_match_tests, literal_pattern_matches_only_exact_literal) {
	EXPECT_TRUE(string_wildcard_match("strawberry", "strawberry"));
	EXPECT_FALSE(string_wildcard_match("straw", "strawberry"));
	EXPECT_FALSE(string_wildcard_match("berry", "strawberry"));
}

TEST(wildcard_match_tests, single_asterisk_matches_any_string) {
	EXPECT_TRUE(string_wildcard_match("strawberry", "*"));
	EXPECT_TRUE(string_wildcard_match("straw", "*"));
	EXPECT_TRUE(string_wildcard_match("berry", "*"));
}
