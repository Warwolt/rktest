#ifndef RKTEST_POC_H
#define RKTEST_POC_H

#include <stdio.h>

typedef struct {
	const char* suite_name;
	const char* test_name;
	void (*run)(void);
} test_data_t;

#define TEST(SUITE, NAME)                                                   \
	void SUITE##_##NAME##_impl(void);                                       \
	static const test_data_t SUITE##_##NAME##_data = {                      \
		.suite_name = #SUITE,                                               \
		.test_name = #NAME,                                                 \
		.run = &SUITE##_##NAME##_impl,                                      \
	};                                                                      \
	DEFINE_SECTION_BEGIN                                                    \
	const test_data_t* const SUITE##_##NAME##_ptr = &SUITE##_##NAME##_data; \
	DEFINE_SECTION_END;                                                     \
	void SUITE##_##NAME##_impl(void)

#if defined(_MSC_VER)
#define DEFINE_SECTION_BEGIN                \
	__pragma(data_seg(push));               \
	__pragma(section("rktest$data", read)); \
	__declspec(allocate("rktest$data"))
#elif defined(__APPLE__)
#define DEFINE_SECTION_BEGIN __attribute__((used, section("__DATA,rktest")))
#elif defined(__unix__)
#define DEFINE_SECTION_BEGIN __attribute__((used, section("rktest")))
#endif

#if defined(_MSC_VER)
#define DEFINE_SECTION_END __pragma(data_seg(pop))
#elif defined(__APPLE__)
#define DEFINE_SECTION_END
#elif defined(__unix__)
#define DEFINE_SECTION_END
#endif

#endif /* RKTEST_POC_H */
