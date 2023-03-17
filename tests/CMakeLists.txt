include(FetchContent)

FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://gitee.com/acking-you/doctest.git
        GIT_TAG v2.4.9
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(doctest)

FetchContent_Declare(
        nanobench
        GIT_REPOSITORY https://github.com/martinus/nanobench.git
        GIT_TAG v4.1.0
        GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(nanobench)
add_definitions(-DBASE_DIR="../../")

file(GLOB TEST_FILES
        "${PROJECT_SOURCE_DIR}/tests/*_test.cc"
        )
file(GLOB BENCH_FILES
        "${PROJECT_SOURCE_DIR}/tests/*_bench.cc"
        )

add_executable(unittest ${TEST_FILES})
target_link_libraries(unittest PRIVATE doctest_with_main ejson nanobench)
target_include_directories(unittest PRIVATE ${PROJECT_SOURCE_DIR}/ejson)
add_test(NAME ${PROJECT_NAME}-unittest COMMAND unittest)

add_executable(benchtest ${BENCH_FILES})
target_link_libraries(benchtest PRIVATE doctest_with_main ejson nanobench)
target_include_directories(benchtest PRIVATE ${PROJECT_SOURCE_DIR}/ejson)
add_test(NAME ${PROJECT_NAME}-benchtest COMMAND benchtest)