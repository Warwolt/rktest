#include <stdio.h>

/* Example of automatic test registration.

   This approach uses a data section ‘rktest’ to store multiple static instances
   of type ‘foo’. These objects are first statically instantiated at the file
   scope and a pointer is then stored in the data section, between some given
   ‘start’ and ‘end’ locations. Objects stored this way can then be retrieved
   at runtime by iterating over all the pointers found between these ‘start’
   and ‘end’ locations.
*/

typedef struct {
	const char* suite_name;
	const char* test_name;
	void (*run)(void);
} test_data_t;

/* Implementation Details                                                     */
/* -------------------------------------------------------------------------- */

#if defined(_MSC_VER)
__pragma(section("rktest$begin", read));
__pragma(section("rktest$data", read));
__pragma(section("rktest$end", read));

__declspec(allocate("rktest$begin")) extern const test_data_t* const rktest_begin = NULL;
__declspec(allocate("rktest$end")) extern const test_data_t* const rktest_end = NULL;

#define DEFINE_SECTION \
	__declspec(allocate("rktest$data"))
#elif defined(__APPLE__)
extern const test_data_t* const
	__start_rktest __asm("section$start$__DATA$rktest");
extern const test_data_t* const
	__stop_rktest __asm("section$end$__DATA$rktest");

#define DEFINE_SECTION \
	__attribute__((used, section("__DATA,rktest")))

DEFINE_SECTION
const test_data_t* const dummy = NULL;
#elif defined(__unix__)
extern const test_data_t* const __start_rktest;
extern const test_data_t* const __stop_rktest;

#define DEFINE_SECTION \
	__attribute__((used, section("rktest")))

DEFINE_SECTION
const test_data_t* const dummy = NULL;
#endif

/* Public API                                                                 */
/* -------------------------------------------------------------------------- */

#if defined(_MSC_VER)
#define TEST_DATA_BEGIN \
	(&rktest_begin + 1)
#define TEST_DATA_END \
	(&rktest_end)
#elif defined(__unix__) || defined(__APPLE__)
#define TEST_DATA_BEGIN \
	(&__start_rktest)
#define TEST_DATA_END \
	(&__stop_rktest)
#endif

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

/* Usage                                                                      */
/* -------------------------------------------------------------------------- */

TEST(hello, alice) {
	printf("Hello Alice\n");
}

TEST(hello, bob) {
	printf("Hello Bob\n");
}

TEST(hello, charlie) {
	printf("Hello Charlie\n");
}

int main(void) {
	const test_data_t* const* it;

	for (it = TEST_DATA_BEGIN; it < TEST_DATA_END; ++it) {
		if (*it) {
			printf("[ RUN      ] %s.%s\n", (*it)->suite_name, (*it)->test_name);
			(*it)->run();
			printf("[       OK ] %s.%s\n", (*it)->suite_name, (*it)->test_name);
		}
	}

	return 0;
}
