include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
        GIT_SHALLOW TRUE
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
        nanobench
        GIT_REPOSITORY https://gitee.com/acking-you/nanobench.git
        GIT_TAG v4.1.0
        GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(nanobench)
add_definitions(-DJSON_DIR="${PROJECT_SOURCE_DIR}/tests/json")

file(GLOB TEST_FILES
        "${PROJECT_SOURCE_DIR}/tests/*_test.cc"
)
file(GLOB BENCH_FILES
        "${PROJECT_SOURCE_DIR}/tests/*_bench.cc"
)

add_executable(unittest ${TEST_FILES})
target_link_libraries(unittest PRIVATE GTest::gtest_main ejson nanobench)
target_include_directories(unittest PRIVATE ${PROJECT_SOURCE_DIR}/ejson)
add_test(NAME ${PROJECT_NAME}-unittest COMMAND unittest)

add_executable(benchtest ${BENCH_FILES})
target_link_libraries(benchtest PRIVATE GTest::gtest_main ejson nanobench)
target_include_directories(benchtest PRIVATE ${PROJECT_SOURCE_DIR}/ejson)