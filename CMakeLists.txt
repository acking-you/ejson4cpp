cmake_minimum_required(VERSION 3.14)
project(my-json)

set(CMAKE_CXX_STANDARD 17)

add_library(my-json my-json/jobject.cc my-json/parser.cc)

add_subdirectory(tests)
