cmake_minimum_required(VERSION 3.16.0)
project(rktest)
enable_language(C)
include(CheckCCompilerFlag)

option(rktest_build_tests "Build rktest tests" OFF)
option(rktest_build_samples "Build rktest samples" OFF)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Test Library
add_library(rktest src/rktest.c)
target_include_directories(rktest PUBLIC include)
target_compile_definitions(rktest PRIVATE RKTEST_DEFINE_MAIN=1)
set_property(TARGET rktest PROPERTY C_STANDARD 99)

if(UNIX)
    target_link_libraries(rktest PUBLIC m)
endif()

# Tests
if (rktest_build_tests)
    set(TEST_SRC
        tests/disabled_tests.c
        tests/float_tests.c
        tests/integer_tests.c
        tests/string_tests.c
        tests/wildcard_match_tests.c
    )
    # Passing tests
    add_executable(tests ${TEST_SRC})
    target_link_libraries(tests PUBLIC rktest)
    # Failing tests
    add_executable(failing_tests ${TEST_SRC})
    target_link_libraries(failing_tests PUBLIC rktest)
    target_compile_definitions(failing_tests PRIVATE RKTEST_FAILING_TESTS=1)
endif (rktest_build_tests)

# Samples
if (rktest_build_samples)
    add_executable(sample1 samples/sample01_factorial.c)
    target_link_libraries(sample1 PUBLIC rktest)
endif (rktest_build_samples)
