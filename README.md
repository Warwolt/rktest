# RK Test

RK Test is a small test library for C99 programs, written to closely mimick
[Google Test](https://github.com/google/googletest). RK Test is lightweight and
easy to integrate into a C project to add testing capabilities,  as it consists
of only a single source and single header file.

## Features
RK Test has the following features:
- Small and easy to integrate, one '\*.c' file and one '\*.h' file
- Supports Windows, MacOS and Linux.
- Self registering tests (relying on a compiler extension common to MSVC, AppleClang and GCC)
- xUnit style assertions and test reporting very close to Google Test

## Example usage
To set up a simple test suite, create a source file that includes `rktest.h` and link against the `rktest_main` library.

Test cases are then defined with the `TEST()` macro, which takes to arguments: the name of the test suite, and the name of the test case. A body is then given, which will be executed like a function. The `EXPECT_EQ` takes two integral types and compares them, and fails the test in case they are not equal.

```C
#include <rktest/rktest.h>

int factorial(int n) {
	int result = 1;
	for (int i = 1; i <= n; i++) {
		result *= i;
	}

	return result;
}

TEST(simple_tests, factorial_of_negative_is_one) {
	EXPECT_EQ(factorial(-1), 1);
	EXPECT_EQ(factorial(-42), 1);
}

TEST(simple_tests, factorial_of_zero_is_one) {
	EXPECT_EQ(factorial(0), 1);
}

TEST(simple_tests, factorial_of_positive_numbers) {
	EXPECT_EQ(factorial(3), 6);
	EXPECT_EQ(factorial(4), 24);
}
```

Building and running the above gives the following output:

![Sample1 output](/samples/sample01_output.png)

We can modify the `factorial` function to be faulty to observe the error messages printed when the tests fail.

```diff
int factorial(int n) {
	int result = 1;
-	for (int i = 1; i <= n; i++) {
+	for (int i = 1; i < n; i++) {
		result *= i;
	}

	return result;
}
```

Rerunning the test suite now gives:

![Sample1 failing output](/samples/sample01_output_failing.png)

## Integrating
(build from source, or link against)

## Assertions
(document all macros)

## Why use RK Test instead of Google Test?

While Google Test is a much more mature test library, it's written in C++. This means that when testing C code, it requires calling that C code from C++ code, which can be awkward due to requiring `extern C` or lacking C99-style designated initializers.

Additionally, the Google Test source is many thousands of lines long, which
makes it a non-trivial dependency to add to a project.

Comparatively, RK Test is lightweight at just around 1k lines of code in just
one `*.c` file and one `*.h` and is written in C, so both code under test and
the test cases are written in the same language. This allows RK Test to easily
be integrated into other C projects, while keeping largely the same development
experience as Google Test.

## See also

Here's a list of other unit test frameworks built on similar techniques:

- [Google Test](https://github.com/google/googletest)
- [Criterion](https://github.com/Snaipe/Criterion/)
- [Rexo](https://github.com/christophercrouzet/rexo/)

## Running the snapshot tests of RK Test

RK Test uses python and snapshot tests to test the output from the library. To run the snapshot tests, first install python:

- https://www.python.org/downloads/

Then, install pytest:

```
pip install pytest syrupy
```

Finally, run the tests from repo root:

```
pytest
```

If a snapshot needs to be updated, run:

```
pytest --snapshot-update
```

And commit the updated snapshots.
