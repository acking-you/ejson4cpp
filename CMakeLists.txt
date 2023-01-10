cmake_minimum_required(VERSION 3.14)
project(ejson)

set(CMAKE_CXX_STANDARD 14)

add_library(ejson ${PROJECT_SOURCE_DIR}/src/ejson/jobject.cc ${PROJECT_SOURCE_DIR}/src/ejson/parser.cc)
target_include_directories(ejson PUBLIC ${PROJECT_SOURCE_DIR}/src)
option(EJSON_UNITTEST "Build unittest." ON)
if(EJSON_UNITTEST)
    add_subdirectory(tests)
endif()
