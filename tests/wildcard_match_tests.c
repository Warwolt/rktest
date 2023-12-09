#include <rktest/rktest.h>

static bool string_wildcard_match(const char* str, const char* pattern) {
	if (*str == '\0') {
		return *pattern == '\0';
	}

	return false;
}

TEST(wildcard_match_tests, empty_pattern_matches_only_empty_string) {
	EXPECT_TRUE(string_wildcard_match("", ""));
	EXPECT_FALSE(string_wildcard_match("strawberry", ""));
}
