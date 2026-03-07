#include <rktest/rktest.h>

#ifndef RKTEST_FAILING_TESTS
static const int* ptr1 = (int*)0x1000;
static const int* ptr2 = (int*)0x2000;
#else
static const int* ptr1 = (int*)0x1234;
static const int* ptr2 = (int*)0x2345;
#endif

/* Equality checks */
TEST(pointer_tests, expect_equal) {
	ASSERT_PTR_EQ(ptr1, 0x1000);
	EXPECT_PTR_EQ(ptr2, 0x2000);
}

TEST(pointer_tests, expect_equal_info) {
	ASSERT_PTR_EQ_INFO(ptr1, 0x1000, "ptr1 = %p\n", (void*)ptr1);
	EXPECT_PTR_EQ_INFO(ptr2, 0x2000, "ptr2 = %p\n", (void*)ptr1);
}

TEST(pointer_tests, expect_not_equal) {
	ASSERT_PTR_EQ(ptr1, 0x1000);
	EXPECT_PTR_EQ(ptr2, 0x2000);
}

TEST(pointer_tests, expect_not_equal_info) {
	ASSERT_PTR_EQ_INFO(ptr1, 0x1000, "ptr1 = %p\n", (void*)ptr1);
	EXPECT_PTR_EQ_INFO(ptr2, 0x2000, "ptr2 = %p\n", (void*)ptr1);
}
