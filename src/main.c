#include <stdio.h>

/* Example of automatic test registration.

   This approach uses a data section ‘bar’ to store multiple static instances
   of type ‘foo’. These objects are first statically instantiated at the file
   scope and a pointer is then stored in the data section, between some given
   ‘start’ and ‘end’ locations. Objects stored this way can then be retrieved
   at runtime by iterating over all the pointers found between these ‘start’
   and ‘end’ locations.
*/

struct foo {
	int value;
};

/* Implementation Details                                                     */
/* -------------------------------------------------------------------------- */

#if defined(_MSC_VER)
__pragma(section("bar$a", read))
	__pragma(section("bar$b", read))
		__pragma(section("bar$c", read))

			__declspec(allocate("bar$a")) extern const struct foo* const bar_begin = NULL;

__declspec(allocate("bar$c")) extern const struct foo* const bar_end = NULL;

#define DEFINE_SECTION \
	__declspec(allocate("bar$b"))
#elif defined(__APPLE__)
extern const struct foo* const
	__start_bar __asm("section$start$__DATA$bar");
extern const struct foo* const
	__stop_bar __asm("section$end$__DATA$bar");

#define DEFINE_SECTION \
	__attribute__((used, section("__DATA,bar")))

DEFINE_SECTION
const struct foo* const dummy = NULL;
#elif defined(__unix__)
extern const struct foo* const __start_bar;
extern const struct foo* const __stop_bar;

#define DEFINE_SECTION \
	__attribute__((used, section("bar")))

DEFINE_SECTION
const struct foo* const dummy = NULL;
#endif

/* Public API                                                                 */
/* -------------------------------------------------------------------------- */

#if defined(_MSC_VER)
#define SECTION_BEGIN \
	(&bar_begin + 1)
#define SECTION_END \
	(&bar_end)
#elif defined(__unix__) || defined(__APPLE__)
#define SECTION_BEGIN \
	(&__start_bar)
#define SECTION_END \
	(&__stop_bar)
#endif

#define REGISTER_FOO(id, value)             \
	static const struct foo id = { value }; \
	DEFINE_SECTION                          \
	extern const struct foo* const id##_ptr = &id

/* Usage                                                                      */
/* -------------------------------------------------------------------------- */

REGISTER_FOO(b, 234);
REGISTER_FOO(a, 123);
REGISTER_FOO(c, 345);

int main(
	void
) {
	const struct foo* const* it;

	for (it = SECTION_BEGIN; it < SECTION_END; ++it) {
		if (*it != NULL) {
			printf("%d\n", (*it)->value);
		}
	}

	return 0;
}
