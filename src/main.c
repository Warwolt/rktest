#include <stdio.h>

typedef struct {
	void (*test_case)(void);
} rk_test_case_t;

/* Implementation Details                                                     */
/* -------------------------------------------------------------------------- */

#if defined(_MSC_VER)
__pragma(section("rk_testcases$begin", read));
__pragma(section("rk_testcases$case", read));
__pragma(section("rk_testcases$end", read));
__declspec(allocate("rk_testcases$begin")) extern const rk_test_case_t* const rk_testcases_begin = NULL;
__declspec(allocate("rk_testcases$end")) extern const rk_test_case_t* const rk_testcases_end = NULL;

#define DEFINE_SECTION \
	__declspec(allocate("rk_testcases$case"))
#elif defined(__APPLE__)
extern const rk_test_case_t* const
	__start_rk_testcases __asm("section$start$__DATA$rk_testcases");
extern const rk_test_case_t* const
	__stop_rk_testcases __asm("section$end$__DATA$rk_testcases");

#define DEFINE_SECTION \
	__attribute__((used, section("__DATA,rk_testcases")))

DEFINE_SECTION
extern const rk_test_case_t* const dummy = NULL;
#elif defined(__unix__)
extern const rk_test_case_t* const __start_rk_testcases;
extern const rk_test_case_t* const __stop_rk_testcases;

#define DEFINE_SECTION \
	__attribute__((used, section("rk_testcases")))

DEFINE_SECTION
extern const rk_test_case_t* const dummy = NULL;
#endif

/* Public API                                                                 */
/* -------------------------------------------------------------------------- */

#if defined(_MSC_VER)
#define TESTS_BEGIN \
	(&rk_testcases_begin + 1)
#define TESTS_END \
	(&rk_testcases_end)
#elif defined(__unix__) || defined(__APPLE__)
#define TESTS_BEGIN \
	(&__start_rk_testcases)
#define TESTS_END \
	(&__stop_rk_testcases)
#endif

#define TEST(name)                                         \
	void rk_test__##name(void);                            \
	static const rk_test_case_t name = {                   \
		.test_case = &rk_test__##name                      \
	};                                                     \
	DEFINE_SECTION                                         \
	extern const rk_test_case_t* const name##_ptr = &name; \
	void rk_test__##name(void)

/* Usage                                                                      */
/* -------------------------------------------------------------------------- */

TEST(hello_world) {
	printf("Hello World!\n");
}

void run_tests() {
	for (const rk_test_case_t* const* it = TESTS_BEGIN; it < TESTS_END; it++) {
		if (*it) {
			(*it)->test_case();
		}
	}
}

int main(void) {
	run_tests();
	return 0;
}
