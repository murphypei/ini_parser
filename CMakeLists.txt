cmake_minimum_required(VERSION 2.8)

PROJECT(ini_reader)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall")

add_executable(ini_reader_test
        ${CMAKE_SOURCE_DIR}/ini_reader_test.cpp
        ${CMAKE_SOURCE_DIR}/ini_reader.cpp
        ${CMAKE_SOURCE_DIR}/ini.c)
target_include_directories(ini_reader_test PUBLIC
        ${CMAKE_SOURCE_DIR}/)