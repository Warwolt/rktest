#include <rktest/rktest.h>

#include <stdio.h>
#include <stdlib.h>

#include <windows.h>

#define ASSERT_DEATH(expr, message)                \
	if (rktest_death_test_line() == __LINE__) {    \
		printf("%s %d\n", __FUNCTION__, __LINE__); \
		expr;                                      \
	}

// TEST(death_tests, foo) {
// 	ASSERT_DEATH({
//         fprintf(stderr, "Error!\n");
// 		exit(1);
//     }, "Error!");
// }

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

	/* Wait for process to finish, then get exit code */
	WaitForSingleObject(process_info.hProcess, INFINITE);
	GetExitCodeProcess(process_info.hProcess, (LPDWORD)exit_code);

	/* Cleanup */
	CloseHandle(stderr_read);
	CloseHandle(process_info.hProcess);
	CloseHandle(process_info.hThread);
}

TEST(death_tests, foo) {
	const int line = 23; // __LINE__
	if (rktest_death_test_line() == line) {
		// run death test block
		printf("%s %d\n", __FUNCTION__, line);
		{
			fprintf((__acrt_iob_func(2)), "Hello world!\n");
			exit(1);
		};
	} else if (rktest_death_test_line() == 0) {
		char test_exe[MAX_PATH];
		GetModuleFileName(NULL, test_exe, MAX_PATH);

		char command[MAX_PATH];
		snprintf(command, MAX_PATH, "%s --rktest_filter=\"%s\" --rktest_death_line=%d", test_exe, "death_tests.foo", line);

		char stderr_buf[1024];
		int exit_code = 0;
		run_command(command, &exit_code, stderr_buf, sizeof(stderr_buf));

		printf("DEBUG: %s", stderr_buf);
		printf("DEBUG: %d\n", exit_code);
	}
}
