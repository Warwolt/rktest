#include <rktest/rktest.h>

// NOTE: this function should be inlined into rktest.c if changed here

// Based on "EnhancedMaskTest" function in 7zip source code
// https://github.com/mcmilk/7-Zip/blob/master/CPP/Common/Wildcard.cpp
static bool string_wildcard_match(const char* str, const char* pattern) {
	while (true) {
		if (pattern[0] == 0) {
			return (str[0] == 0);
		}
		if (pattern[0] == '*') {
			if (string_wildcard_match(str, pattern + 1)) {
				return true;
			}
			if (str[0] == 0) {
				return false;
			}
		} else {
			if (pattern[0] == '?') {
				if (str[0] == 0) {
					return false;
				}
			} else if (pattern[0] != str[0]) {
				return false;
			}
			pattern++;
		}
		str++;
	}
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

TEST(wildcard_match_tests, literal_then_asterisk_does_prefix_match) {
	EXPECT_TRUE(string_wildcard_match("straw", "straw*"));
	EXPECT_TRUE(string_wildcard_match("strawberry", "straw*"));
	EXPECT_FALSE(string_wildcard_match("stra", "straw*"));
	EXPECT_FALSE(string_wildcard_match("blueberry", "straw*"));
}

TEST(wildcard_match_tests, asterisk_then_literal_does_suffix_match) {
	EXPECT_TRUE(string_wildcard_match("berry", "*berry"));
	EXPECT_TRUE(string_wildcard_match("strawberry", "*berry"));
	EXPECT_FALSE(string_wildcard_match("straw", "*berry"));
}

TEST(wildcard_match_tests, prefix_and_suffix_match) {
	EXPECT_TRUE(string_wildcard_match("strawberry", "st*ry"));
	EXPECT_FALSE(string_wildcard_match("strawberry", "s*r"));
}

TEST(wildcard_match_tests, infix_match) {
	EXPECT_TRUE(string_wildcard_match("wildcard_match_tests.empty_pattern_matches_only_empty_string", "*tests*"));
}
