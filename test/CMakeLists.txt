cmake_minimum_required(VERSION 3.8)
project(test_task LANGUAGES CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Release)

set(PROJECT_SOURCES
        main.cpp
        cpptest.hpp
        ../main/csv_parser.hpp
        ../main/csv_parser.cpp
)

add_executable(tests ${PROJECT_SOURCES})
add_custom_command(TARGET tests PRE_BUILD 
                    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/test_csv_cases 
                    $<TARGET_FILE_DIR:tests>/test_csv_cases)