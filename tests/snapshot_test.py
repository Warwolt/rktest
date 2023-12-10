import os
import subprocess

TEST_EXECUTABLE = './build/Debug/tests' if os.name == 'nt' else './build/tests'
FAILING_TEST_EXECUTABLE = './build/Debug/failing_tests' if os.name == 'nt' else './build/failing_tests'


def run_test_exe(exe, args: [str] = []) -> str:
    result = subprocess.run([exe, '--rktest_print_time=0', '--rktest_print_filenames=0',
                            '--rktest_color=no'] + args, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    print('cmd:', ' '.join(result.args))
    return result.stdout


def test_no_args(snapshot):
    actual = run_test_exe(TEST_EXECUTABLE)
    assert actual == snapshot


def test_wildcard_match(snapshot):
    actual = run_test_exe(TEST_EXECUTABLE, ['--rktest_filter=*'])
    assert actual == snapshot


def test_suffix_match(snapshot):
    actual = run_test_exe(TEST_EXECUTABLE, ['--rktest_filter=*equal'])
    assert actual == snapshot


def test_prefix_match(snapshot):
    actual = run_test_exe(TEST_EXECUTABLE, ['--rktest_filter=integer*'])
    assert actual == snapshot


def test_infix_match(snapshot):
    actual = run_test_exe(TEST_EXECUTABLE, ['--rktest_filter=*tests*'])
    assert actual == snapshot


def test_failing_tests(snapshot):
    actual = run_test_exe(FAILING_TEST_EXECUTABLE)
    assert actual == snapshot


def test_print_help(snapshot):
    actual = run_test_exe(TEST_EXECUTABLE, ['--help'])
    assert actual == snapshot


def test_pass_bad_arg(snapshot):
    actual = run_test_exe(TEST_EXECUTABLE, ['--badargument'])
    assert actual == snapshot
