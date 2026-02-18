#include <rktest/rktest.h>

#ifdef RKTEST_FAILING_TESTS

TEST(fail_tests, fail_test) {
	FAIL();
}

TEST(fail_tests, fail_info_test) {
	FAIL_INFO("Hello world\n");
}

TEST(fail_tests, add_failiure_test) {
	ADD_FAILURE();
	EXPECT_EQ(2 + 2, 5); // this should run and fail
}

TEST(fail_tests, add_failiure_info_test) {
	ADD_FAILURE_INFO("Hello world\n");
	EXPECT_EQ(2 + 2, 5); // this should run and fail
}

#endif // RKTEST_FAILING_TESTS
