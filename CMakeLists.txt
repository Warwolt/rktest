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
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 99)
target_include_directories(${PROJECT_NAME} PUBLIC
    include
)
if(UNIX)
    target_link_libraries(${PROJECT_NAME} PUBLIC m)
endif()

# Tests
set(TEST_SRC
    tests/disabled_tests.c
    tests/float_tests.c
    tests/integer_tests.c
    tests/string_tests.c
    tests/wildcard_match_tests.c
)
add_executable(tests ${TEST_SRC})
target_include_directories(tests PUBLIC include)
target_link_libraries(tests PUBLIC ${PROJECT_NAME})

add_executable(failing_tests ${TEST_SRC})
target_include_directories(failing_tests PUBLIC include)
target_link_libraries(failing_tests PUBLIC ${PROJECT_NAME})
target_compile_definitions(failing_tests PRIVATE RKTEST_FAILING_TESTS=1)

# Samples
add_executable(sample1 samples/sample01_factorial.c)
target_include_directories(sample1 PUBLIC include)
target_link_libraries(sample1 PUBLIC ${PROJECT_NAME})
