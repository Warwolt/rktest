#include <stddef.h>

typedef struct {
	const char* name;
	const char* suite;
	void (*func)(void);
} rktest_test_t;

#define TEST(SUITE, NAME)                                                                                                      \
	__pragma(data_seg(push));                                                                                                  \
	__pragma(section("rktestdata$data", read));                                                                                \
	void SUITE##_##NAME##_impl(void);                                                                                          \
	const rktest_test_t SUITE##_##NAME##_data = {                                                                              \
		.name = #NAME,                                                                                                         \
		.suite = #SUITE,                                                                                                       \
		.func = &SUITE##_##NAME##_impl                                                                                         \
	};                                                                                                                         \
	__declspec(allocate("rktestdata$data")) const rktest_test_t* const SUITE##_##NAME##_data##_##ptr = &SUITE##_##NAME##_data; \
	__pragma(data_seg(pop));                                                                                                   \
	void SUITE##_##NAME##_impl(void)

#define EXPECT_EQ(lhs, rhs)
#define EXPECT_GT(lhs, rhs)
#define EXPECT_TRUE(lhs)
#define EXPECT_FALSE(lhs)

int rktest_main(void);
