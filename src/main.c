#include <stdio.h>

#include "rktest_poc.h"

/* Declare memory section to store test data in */
#if defined(_MSC_VER)
__pragma(section("rktest$begin", read));
__pragma(section("rktest$data", read));
__pragma(section("rktest$end", read));
__declspec(allocate("rktest$begin")) extern const test_data_t* const rktest_begin = NULL;
__declspec(allocate("rktest$end")) extern const test_data_t* const rktest_end = NULL;

#elif defined(__APPLE__)

extern const test_data_t* const __start_rktest __asm("section$start$__DATA$rktest");
extern const test_data_t* const __stop_rktest __asm("section$end$__DATA$rktest");
__attribute__((used, section("__DATA,rktest"))) const test_data_t* const dummy = NULL;

#elif defined(__unix__)

extern const test_data_t* const __start_rktest;
extern const test_data_t* const __stop_rktest;
__attribute__((used, section("rktest"))) const test_data_t* const dummy = NULL;
#endif

#if defined(_MSC_VER)
#define TEST_DATA_BEGIN (&rktest_begin + 1)
#define TEST_DATA_END (&rktest_end)
#elif defined(__unix__) || defined(__APPLE__)
#define TEST_DATA_BEGIN (&__start_rktest)
#define TEST_DATA_END (&__stop_rktest)
#endif

int main(void) {
	for (const test_data_t* const* it = TEST_DATA_BEGIN; it < TEST_DATA_END; ++it) {
		if (*it) {
			printf("[ RUN      ] %s.%s\n", (*it)->suite_name, (*it)->test_name);
			(*it)->run();
			printf("[       OK ] %s.%s\n", (*it)->suite_name, (*it)->test_name);
		}
	}

	return 0;
}
