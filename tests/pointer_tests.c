#include <rktest/rktest.h>

static const int value1 = 0;
static const int value2 = 0;

#ifndef RKTEST_FAILING_TESTS
static const int* ptr1 = &value1;
static const int* ptr2 = &value2;
#else
static const int* ptr1 = &value2;
static const int* ptr2 = &value1;
#endif

/* Equality checks */
TEST(pointer_tests, expect_equal) {
	ASSERT_PTR_EQ(ptr1, &value1);
	EXPECT_PTR_EQ(ptr2, &value2);
}

TEST(pointer_tests, expect_equal_info) {
	ASSERT_PTR_EQ_INFO(ptr1, &value1, "ptr1 = %p\n", ptr1);
	EXPECT_PTR_EQ_INFO(ptr2, &value2, "ptr2 = %p\n", ptr1);
}

TEST(pointer_tests, expect_not_equal) {
	ASSERT_PTR_EQ(ptr1, &value1);
	EXPECT_PTR_EQ(ptr2, &value2);
}

TEST(pointer_tests, expect_not_equal_info) {
	ASSERT_PTR_EQ_INFO(ptr1, &value1, "ptr1 = %p\n", ptr1);
	EXPECT_PTR_EQ_INFO(ptr2, &value2, "ptr2 = %p\n", ptr1);
}
