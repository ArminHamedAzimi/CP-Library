cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_COMPILER "g++-15")

project(cp-library CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-O2 -Wall -Wextra -std=c++20")
set(CMAKE_CXX_FLAGS_DEBUG "-g -fsanitize=address -fsanitize=undefined -DLOCAL")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

include_directories(src)
include_directories(template)

enable_testing()

add_library(test_runner STATIC test/test_runner.cpp)

file(GLOB_RECURSE ALL_TEST_SOURCES "test/**/test_*.cpp")

foreach(test_file ${ALL_TEST_SOURCES})
    get_filename_component(test_name ${test_file} NAME_WE)
    get_filename_component(test_dir ${test_file} DIRECTORY)
    get_filename_component(module_name ${test_dir} NAME)
    
    set(full_test_name "${module_name}_${test_name}")
    
    add_executable(${full_test_name} ${test_file})
    target_link_libraries(${full_test_name} test_runner)
    
    add_test(NAME ${full_test_name} COMMAND ${full_test_name})
    
    message(STATUS "Created test: ${full_test_name}")
endforeach()

add_custom_target(unit_tests
    COMMAND ctest --output-on-failure -R "test_"
    COMMENT "Running all unit tests"
)

add_custom_target(data_structures_tests
    COMMAND ctest --output-on-failure -R "data-structures"
    COMMENT "Running data structures tests"
)

add_custom_target(graph_tests
    COMMAND ctest --output-on-failure -R "graph"
    COMMENT "Running graph algorithm tests"
)

add_custom_target(math_tests
    COMMAND ctest --output-on-failure -R "math"
    COMMENT "Running math algorithm tests"
)

add_custom_target(string_tests
    COMMAND ctest --output-on-failure -R "string"
    COMMENT "Running string algorithm tests"
)

add_custom_target(all_tests
    COMMAND ctest --output-on-failure
    COMMENT "Running ALL tests"
)

install(DIRECTORY src/
    DESTINATION include/cp-library
    FILES_MATCHING PATTERN "*.hpp"
)

install(FILES template/template.cpp template/debug.h
    DESTINATION include/cp-library/template
)

message(STATUS "=== Configuration Summary ===")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Flags: ${CMAKE_CXX_FLAGS}")

list(LENGTH ALL_TEST_SOURCES num_tests)
message(STATUS "=== Test Configuration ===")
message(STATUS "Found ${num_tests} test files")
