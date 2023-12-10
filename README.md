# RK Test

A small test library for C programs, written to closely mimick [Google Test](https://github.com/google/googletest). This is intended as a library that is lightweight and easy to integrate into a C project to add testing capabilities.

RK Test has the following features:
- Self registering tests
- Google Test style test reporting and asserts

## See also

Here's a list of other unit test frameworks built on similar techniques:

- [Google Test](https://github.com/google/googletest)
- [Criterion](https://github.com/Snaipe/Criterion/)
- [Rexo](https://github.com/christophercrouzet/rexo/)

## Automatic tests of RK Test

RK Test tests itself using a combination of C++ and Python tests. To run the tests, first install python:

- https://www.python.org/downloads/

Then, install pytest:

```
pip install pytest
```

Finally, run the tests from repo root:

```
pytest
```
