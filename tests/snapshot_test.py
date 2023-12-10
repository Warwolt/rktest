import os
import subprocess

TEST_EXECUTABLE = './build/Debug/tests' if os.name == 'nt' else './build/tests'


def run_test_exe(args: [str] = []) -> str:
    result = subprocess.run([TEST_EXECUTABLE, '--rktest_print_time=0'] + args, stdout=subprocess.PIPE)
    return result.stdout.decode('utf-8')


def test_no_args(snapshot):
    actual = run_test_exe()
    assert actual == snapshot
