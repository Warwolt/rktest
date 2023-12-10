#include <rktest/rktest.h>

static bool string_wildcard_match(const char* str, const char* pattern) {
	while (true) {
		/* If at end of either string, stop iterating */
		if (str[0] == '\0' || pattern[0] == '\0') {
			return pattern[0] == str[0] || pattern[0] == '*';
		}

		/* If asterisk, try to match next chars */
		if (pattern[0] == '*') {
			while (true) {
				// if pattern doesn't have a suffix, anything will match
				if (pattern[1] == '\0') {
					return true;
				}
				// if string is empty, we didn't find a match
				if (str[0] == '\0') {
					return false;
				}
				// matched pattern
				if (str[0] == pattern[1]) {
					str += 1;
					pattern += 2;
					break;
				}
				str++;
			}
		}

		/* If current chars differ, then does not match */
		else if (str[0] != pattern[0]) {
			return false;
		}

		/* Go to next characters*/
		str++;
		if (pattern[0] != '*') {
			pattern++;
		}
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
