cmake_minimum_required(VERSION 3.16.0)
project(rktest)
enable_language(C)
include(CheckCCompilerFlag)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Test Library
add_library(${PROJECT_NAME}
    src/rktest_main.c
    src/rktest.c
)
target_include_directories(${PROJECT_NAME} PUBLIC
    include
)
if(UNIX)
    target_link_libraries(${PROJECT_NAME} PUBLIC m)
endif()

# Tests
add_executable(tests
    tests/disabled_tests.c
    # tests/float_tests.c
    # tests/integer_tests.c
    # tests/string_tests.c
)
target_include_directories(tests PUBLIC include)
target_link_libraries(tests PUBLIC ${PROJECT_NAME})
