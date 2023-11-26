// based on https://github.com/google/googletest/blob/main/googletest/samples/sample1.cc

#include "sample1_factorial.h"

int factorial(int n) {
	int result = 1;
	for (int i = 1; i <= n; i++) {
		result *= i;
	}

	return result;
}

bool is_prime(int n) {
	if (n <= 1)
		return false;

	if (n % 2 == 0)
		return n == 2;

	for (int i = 3;; i += 2) {
		// We only have to try i up to the square root of n
		if (i > n / i)
			break;

		// Now, we have i <= n/i < n.
		// If n is divisible by i, n is not prime.
		if (n % i == 0)
			return false;
	}

	// n has no integer factor in the range (1, n), and thus is prime.
	return true;
}
