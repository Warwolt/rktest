cmake_minimum_required(VERSION 3.16.0)
project(rktest)
enable_language(C)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

check_c_compiler_flag(-Wno-error=missing-braces MISSING_BRACES_WARNING)
if (MISSING_BRACES_WARNING)
    target_compile_options(target_name PRIVATE -Wno-error=missing-braces)
endif (MISSING_BRACES_WARNING)

# Test Library
add_library(${PROJECT_NAME}
    src/rktest_main.c
    src/rktest.c
)

target_include_directories(${PROJECT_NAME} PUBLIC
    include
)

# Samples
add_executable(sample1
    samples/sample1_factorial.c
    samples/sample1_factorial_tests.c
)
target_include_directories(sample1 PUBLIC include)
target_link_libraries(sample1 PUBLIC ${PROJECT_NAME})

# Tests
add_executable(tests
    tests/expect_numeric_tests.c
    tests/assert_numeric_tests.c
)
target_include_directories(tests PUBLIC include)
target_link_libraries(tests PUBLIC ${PROJECT_NAME})
