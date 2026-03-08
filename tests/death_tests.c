#include <rktest/rktest.h>

#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

#pragma warning(disable: 4996) // needed for strncpy

#define ASSERT_DEATH(expr, message)                \
	if (rktest_death_test_line() == __LINE__) {    \
		printf("%s %d\n", __FUNCTION__, __LINE__); \
		expr;                                      \
	}

// TEST(death_tests, foo) {
// 	EXPECT_DEATH({
//         fprintf(stderr, "Error!\n");
// 		exit(1);
//     }, "Error!");
// }

// Based on "EnhancedMaskTest" function in 7zip source code
// https://github.com/mcmilk/7-Zip/blob/master/CPP/Common/Wildcard.cpp
static bool string_wildcard_match(const char* str, const char* pattern) {
	while (true) {
		if (pattern[0] == 0) {
			return (str[0] == 0);
		}
		if (pattern[0] == '*') {
			if (string_wildcard_match(str, pattern + 1)) {
				return true;
			}
			if (str[0] == 0) {
				return false;
			}
		} else {
			if (pattern[0] == '?') {
				if (str[0] == 0) {
					return false;
				}
			} else if (pattern[0] != str[0]) {
				return false;
			}
			pattern++;
		}
		str++;
	}
}

static void run_command(const char* command, int* exit_code, char* stderr_buf, int stderr_buf_size) {
	/* Setup pipes for stdout and stderr */
	HANDLE stderr_read = NULL;
	HANDLE stderr_write = NULL;
	SECURITY_ATTRIBUTES security_attributes = {
		.nLength = sizeof(SECURITY_ATTRIBUTES),
		.lpSecurityDescriptor = NULL,
		.bInheritHandle = TRUE,
	};
	BOOL pipe_was_created = CreatePipe(&stderr_read, &stderr_write, &security_attributes, 0);
	if (!pipe_was_created) {
		fprintf(stderr, "%s:%d CreatePipe failed\n", __FILE__, __LINE__);
		return;
	}

	/* Ensure the read handle to the pipe is not inherited. */
	SetHandleInformation(stderr_read, HANDLE_FLAG_INHERIT, 0);

	/* Create process */
	PROCESS_INFORMATION process_info = { 0 };
	STARTUPINFOA startup_info = {
		.cb = sizeof(STARTUPINFOA),
		.dwFlags = STARTF_USESTDHANDLES,
		.hStdInput = NULL,
		.hStdOutput = NULL,
		.hStdError = stderr_write,
	};
	BOOL process_was_created = CreateProcessA(
		NULL, // lpApplicationName
		(LPSTR)(command), // lpCommandLine
		NULL, // lpProcessAttributes
		NULL, // lpThreadAttributes
		TRUE, // bInheritHandles
		0, // dwCreationFlags
		NULL, // lpEnvironment
		NULL, // lpCurrentDirectory
		&startup_info, // lpStartupInfo
		&process_info // lpProcessInformation
	);

	if (!process_was_created) {
		fprintf(stderr, "%s:%d CreateProcess failed: %d\n", __FILE__, __LINE__, GetLastError());
	}

	/* Close write end of the pipe in the parent process */
	CloseHandle(stderr_write);

	/* Read stderr output */
	if (stderr_buf && stderr_buf_size > 0) {
		DWORD bytes_read = 0;
		int total_read = 0;
		while (total_read < stderr_buf_size - 1) {
			DWORD bytes_to_read = stderr_buf_size - 1 - total_read;

			if (!ReadFile(stderr_read, stderr_buf + total_read, bytes_to_read, &bytes_read, NULL) || bytes_read == 0) {
				break;
			}

			total_read += bytes_read;
		}
		stderr_buf[total_read] = '\0';
	}

	/* Wait for process to finish, then get exit code */
	WaitForSingleObject(process_info.hProcess, INFINITE);
	GetExitCodeProcess(process_info.hProcess, (LPDWORD)exit_code);

	/* Cleanup */
	CloseHandle(stderr_read);
	CloseHandle(process_info.hProcess);
	CloseHandle(process_info.hThread);
}

void rktest_run_death_test(int test_line, bool is_assert, const char* expected_stderr_in) {
	/* Build command */
	char test_exe[MAX_PATH];
	GetModuleFileName(NULL, test_exe, MAX_PATH);
	char command[MAX_PATH];
	snprintf(command, MAX_PATH, "%s --rktest_filter=\"%s.%s\" --rktest_death_line=%d", test_exe, rktest_current_suite_name(), rktest_current_test_name(), test_line);

	/* Run command */
	char actual_stderr[1024];
	int exit_code = 0;
	run_command(command, &exit_code, actual_stderr, sizeof(actual_stderr));

	/* Check that process died */
	if (exit_code == 0) {
		rktest_fail_current_test();
		if (rktest_filenames_enabled()) {
			printf("%s(%d): ", __FILE__, __LINE__);
		}
		printf("error: EXPECT_DEATH failed.\n");
		printf("Expected non-zero exit code, but got 0\n");
		if (is_assert) {
			return;
		}
	}

	/* Strip newline characters from both expected and actual */
	// Windows will output \r\n for newlines, but usually the user is just using \n.
	// To make it saner to compare stderr, we just ignore the newline characters.
	char expected_stderr[1024];
	strncpy(expected_stderr, expected_stderr_in, sizeof(expected_stderr));
	size_t actual_stderr_length = strlen(actual_stderr);
	size_t expected_stderr_length = strlen(expected_stderr);
	if (actual_stderr_length > 0 && actual_stderr[actual_stderr_length - 1] == '\n') {
		actual_stderr[actual_stderr_length - 1] = '\0';
	}
	if (actual_stderr_length > 1 && actual_stderr[actual_stderr_length - 2] == '\r') {
		actual_stderr[actual_stderr_length - 2] = '\0';
	}
	if (expected_stderr_length > 0 && expected_stderr[expected_stderr_length - 1] == '\n') {
		expected_stderr[expected_stderr_length - 1] = '\0';
	}
	if (expected_stderr_length > 1 && expected_stderr[expected_stderr_length - 2] == '\r') {
		expected_stderr[expected_stderr_length - 2] = '\0';
	}

	/* Check expected stderr */
	if (!string_wildcard_match(actual_stderr, expected_stderr)) {
		rktest_fail_current_test();
		if (rktest_filenames_enabled()) {
			printf("%s(%d): ", __FILE__, __LINE__);
		}
		printf("error: EXPECT_DEATH failed.\n");
		printf("Expected stderr output to be:\n");
		printf("  %s\n", expected_stderr);
		printf("But received:\n");
		printf("  %s\n", actual_stderr);
		if (is_assert) {
			return;
		}
	}
}

TEST(death_tests, bar) {
	const int line = 23; // __LINE__
	if (rktest_death_test_line() == line) {
		// run death test block
		printf("%s %d\n", __FUNCTION__, line);
		{
			fprintf(stderr, "Hello world!\n");
			exit(1);
		};
	} else if (rktest_death_test_line() == 0) {
		rktest_run_death_test(line, false, "Hell*!\n");
	}
}
