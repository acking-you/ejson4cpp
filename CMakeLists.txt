cmake_minimum_required(VERSION 3.14)
project(my-json)

set(CMAKE_CXX_STANDARD 17)

add_library(my-json my-json/jobject.cc my-json/parser.cc)
option(BUILD_UNITTEST "Build unittest." OFF)
if(BUILD_UNITTEST)
    add_subdirectory(tests)
endif()
