include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
        GIT_SHALLOW TRUE
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

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
target_link_libraries(unittest PRIVATE gtest_main ejson nanobench)
target_include_directories(unittest PRIVATE ${PROJECT_SOURCE_DIR}/ejson)
add_test(NAME ${PROJECT_NAME}-unittest COMMAND unittest)

add_executable(benchtest ${BENCH_FILES})
target_link_libraries(benchtest PRIVATE gtest_main ejson nanobench)
target_include_directories(benchtest PRIVATE ${PROJECT_SOURCE_DIR}/ejson)
add_test(NAME ${PROJECT_NAME}-benchtest COMMAND benchtest)