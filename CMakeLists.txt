cmake_minimum_required(VERSION 3.16.0)
project(rktest)
enable_language(C)
include(CheckCCompilerFlag)

option(rktest_build_tests "Build rktest tests" OFF)
option(rktest_build_samples "Build rktest samples" OFF)

set(RKTEST "rktest")
set(RKTEST_MAIN "rktest_main")

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Test Library
add_library(${RKTEST} src/rktest.c)
add_library(${RKTEST_MAIN} src/rktest.c)

target_compile_definitions(${RKTEST_MAIN} PRIVATE RKTEST_DEFINE_MAIN=1)

set_property(TARGET ${RKTEST} PROPERTY C_STANDARD 99)
set_property(TARGET ${RKTEST_MAIN} PROPERTY C_STANDARD 99)

target_include_directories(${RKTEST} PUBLIC include)
target_include_directories(${RKTEST_MAIN} PUBLIC include)

if(UNIX)
    target_link_libraries(${RKTEST} PUBLIC m)
    target_link_libraries(${RKTEST_MAIN} PUBLIC m)
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
    target_include_directories(tests PUBLIC include)
    target_link_libraries(tests PUBLIC ${RKTEST_MAIN})
    # Failing tests
    add_executable(failing_tests ${TEST_SRC})
    target_include_directories(failing_tests PUBLIC include)
    target_link_libraries(failing_tests PUBLIC ${RKTEST_MAIN})
    target_compile_definitions(failing_tests PRIVATE RKTEST_FAILING_TESTS=1)
endif (rktest_build_tests)

# Samples
if (rktest_build_samples)
    add_executable(sample1 samples/sample01_factorial.c)
    target_include_directories(sample1 PUBLIC include)
    target_link_libraries(sample1 PUBLIC ${RKTEST_MAIN})
endif (rktest_build_samples)
