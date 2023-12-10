import os
import subprocess

TEST_EXECUTABLE = './build/Debug/tests' if os.name == 'nt' else './build/tests'


def run_test_exe(args: [str] = []) -> str:
    result = subprocess.run([TEST_EXECUTABLE, '--rktest_print_time=0',
                            '--rktest_color=no'] + args, stdout=subprocess.PIPE)
    return result.stdout.decode('utf-8')


def test_no_args(snapshot):
    actual = run_test_exe()
    assert actual == snapshot


def test_wildcard_match(snapshot):
    actual = run_test_exe(['--rktest_filter=*'])
    assert actual == snapshot


def test_suffix_match(snapshot):
    actual = run_test_exe(['--rktest_filter=*equal'])
    assert actual == snapshot


def test_prefix_match(snapshot):
    actual = run_test_exe(['--rktest_filter=integer*'])
    assert actual == snapshot

def test_infix_match(snapshot):
    actual = run_test_exe(['--rktest_filter=*tests*'])
    assert actual == snapshot
