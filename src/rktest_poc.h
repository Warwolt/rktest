#ifndef RKTEST_POC_H
#define RKTEST_POC_H

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
	DEFINE_SECTION                                                          \
	const test_data_t* const SUITE##_##NAME##_ptr = &SUITE##_##NAME##_data; \
	void SUITE##_##NAME##_impl(void)

#if defined(_MSC_VER)
#define DEFINE_SECTION __declspec(allocate("rktest$data"))
#elif defined(__APPLE__)
#define DEFINE_SECTION __attribute__((used, section("__DATA,rktest")))
#elif defined(__unix__)
#define DEFINE_SECTION __attribute__((used, section("rktest")))
#endif

#endif /* RKTEST_POC_H */
