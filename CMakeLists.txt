cmake_minimum_required(VERSION 3.16.0)
project(rktest)
enable_language(C)

add_library(${PROJECT_NAME}
    src/rktest.c
)

add_executable(run_tests
    src/main.c
    tests/foo.c
    tests/bar.c
)

target_include_directories(run_tests PUBLIC
    src
)

target_link_libraries(run_tests PUBLIC
    ${PROJECT_NAME}
)
