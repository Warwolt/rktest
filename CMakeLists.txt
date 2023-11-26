cmake_minimum_required(VERSION 3.16.0)
project(rktest)
enable_language(C)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

add_library(${PROJECT_NAME}
    src/rktest.c
)

add_executable(run_tests
    src/main.c
)

target_include_directories(run_tests PUBLIC
    src
)

target_link_libraries(run_tests PUBLIC
    ${PROJECT_NAME}
)
