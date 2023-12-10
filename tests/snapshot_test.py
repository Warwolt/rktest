import os
import subprocess

TEST_EXECUTABLE = './build/Debug/tests' if os.name == 'nt' else './build/tests'

result = subprocess.run([TEST_EXECUTABLE], stdout=subprocess.PIPE)
output = result.stdout.decode('utf-8')

print(output)

def capital_case(x):
    return x.capitalize()


def test_capital_case():
    assert capital_case('semaphore') == 'Semaphore'
