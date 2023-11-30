cmake_minimum_required(VERSION 3.14)
project(ejson)

#如果ejson目标已经存在,则打印提示并立即返回
if (TARGET ejson)
    message(WARNING "Target ejson already exists!")
    return()
endif ()

# 检测C++版本，并给出提示
if (CMAKE_CXX_STANDARD LESS 11)
    message(FATAL_ERROR "CMAKE_CXX_STANDARD must be at least 11")
elseif (CMAKE_CXX_STANDARD EQUAL 11)
    message(STATUS "Using C++11")
elseif (CMAKE_CXX_STANDARD EQUAL 14)
    message(STATUS "Using C++14")
else ()
    message(STATUS "Using C++17")
    set(CMAKE_CXX_STANDARD 17)
endif ()

add_library(ejson ${PROJECT_SOURCE_DIR}/src/ejson/jobject.cc ${PROJECT_SOURCE_DIR}/src/ejson/parser.cc ${PROJECT_SOURCE_DIR}/src/ejson/base64.cc)
target_include_directories(ejson PUBLIC ${PROJECT_SOURCE_DIR}/src)
option(EJSON_UNITTEST "Build unittest." OFF)
option(EJSON_EXAMPLE "Build example." OFF)
if (EJSON_UNITTEST)
    enable_testing()
    add_subdirectory(tests)
endif ()

if (EJSON_EXAMPLE)
    add_subdirectory(example)
endif ()

