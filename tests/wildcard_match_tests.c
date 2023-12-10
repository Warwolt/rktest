#include <rktest/rktest.h>

// Based on wildcard matching in 7zip source code
// https://github.com/mcmilk/7-Zip/blob/826145b86107fc0a778ac673348226db180e4532/CPP/Common/Wildcard.cpp#L124
static bool string_wildcard_match(const char* str, const char* pattern) {
	while (true) {
		char m = *pattern;
		char c = *str;
		if (m == 0)
			return (c == 0);
		if (m == '*') {
			if (string_wildcard_match(pattern + 1, str))
				return true;
			if (c == 0)
				return 0;
		} else {
			if (m == '?') {
				if (c == 0)
					return false;
			} else if (m != c)
				return false;
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
