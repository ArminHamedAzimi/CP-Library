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

file(GLOB_RECURSE CSES_MAIN_FILES "test/**/cses[0-9]*/main.cpp")

foreach(cses_main ${CSES_MAIN_FILES})
    get_filename_component(cses_dir ${cses_main} DIRECTORY)
    get_filename_component(cses_problem ${cses_dir} NAME)
    get_filename_component(module_dir ${cses_dir} DIRECTORY)
    get_filename_component(module_name ${module_dir} NAME)
    
    set(cses_exe_name "${module_name}_${cses_problem}")
    add_executable(${cses_exe_name} ${cses_main})
    
    target_include_directories(${cses_exe_name} PRIVATE ${CMAKE_SOURCE_DIR}/src)
    target_include_directories(${cses_exe_name} PRIVATE ${CMAKE_SOURCE_DIR}/template)
    
    target_compile_definitions(${cses_exe_name} PRIVATE ONLINE_JUDGE)
    
    file(GLOB test_inputs "${cses_dir}/data/*.in")
    
    if(test_inputs)
        foreach(input_file ${test_inputs})
            get_filename_component(test_case_name ${input_file} NAME_WE)
            set(output_file "${cses_dir}/data/${test_case_name}.out")
            
            if(EXISTS ${output_file})
                set(test_name "${cses_exe_name}_case_${test_case_name}")
                
                add_test(
                    NAME ${test_name}
                    COMMAND ${CMAKE_COMMAND} -E env
                    bash -c "$<TARGET_FILE:${cses_exe_name}> < ${input_file} | diff -w ${output_file} -"
                )
                
                set_tests_properties(${test_name} PROPERTIES
                    LABELS "${module_name};cses;${cses_problem}"
                    TIMEOUT 5
                )
                
                message(STATUS "Created CSES test: ${test_name}")
            else()
                message(WARNING "Output file not found for ${input_file}")
            endif()
        endforeach()
    else()
        message(STATUS "No test cases found for ${cses_problem} in ${cses_dir}/data/")
    endif()
endforeach()

add_custom_target(unit_tests
    COMMAND ctest --output-on-failure -R "test_"
    COMMENT "Running all unit tests"
)

add_custom_target(cses_tests
    COMMAND ctest --output-on-failure -L "cses"
    COMMENT "Running all CSES problem tests"
)

add_custom_target(data_structures_tests
    COMMAND ctest --output-on-failure -R "data-structures"
    COMMENT "Running data structures tests"
)

add_custom_target(data_structures_cses
    COMMAND ctest --output-on-failure -L "data-structures" -L "cses"
    COMMENT "Running data structures CSES tests"
)

add_custom_target(graph_tests
    COMMAND ctest --output-on-failure -R "graph"
    COMMENT "Running graph algorithm tests"
)

add_custom_target(graph_cses
    COMMAND ctest --output-on-failure -L "graph" -L "cses"
    COMMENT "Running graph CSES tests"
)

add_custom_target(math_tests
    COMMAND ctest --output-on-failure -R "math"
    COMMENT "Running math algorithm tests"
)

add_custom_target(math_cses
    COMMAND ctest --output-on-failure -L "math" -L "cses"
    COMMENT "Running math CSES tests"
)

add_custom_target(string_tests
    COMMAND ctest --output-on-failure -R "string"
    COMMENT "Running string algorithm tests"
)

add_custom_target(string_cses
    COMMAND ctest --output-on-failure -L "string" -L "cses"
    COMMENT "Running string CSES tests"
)

add_custom_target(all_tests
    COMMAND ctest --output-on-failure
    COMMENT "Running ALL tests"
)

function(add_cses_runner problem_name)
    add_custom_target(run_${problem_name}
        COMMAND ctest --output-on-failure -L "${problem_name}"
        COMMENT "Running ${problem_name} tests"
    )
endfunction()

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
list(LENGTH CSES_MAIN_FILES num_cses)
message(STATUS "=== Test Configuration ===")
message(STATUS "Found ${num_tests} unit test files")
message(STATUS "Found ${num_cses} CSES problem files")