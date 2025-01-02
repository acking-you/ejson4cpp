cmake_minimum_required(VERSION 3.14)
project(ejson)

# If the ejson target already exists, print a prompt and return immediately
if (TARGET ejson)
    message(WARNING "Target ejson already exists!")
    return()
endif ()

# Detect C++ version and give hints
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

message(STATUS "CMAKE_SYSTEM_PROCESSOR:${CMAKE_SYSTEM_PROCESSOR}")

# Detect the architecture
if (CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64|i386|i686)$")
    set(ARCH_X86 TRUE)
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm|aarch64|arm64)$")
    message(STATUS "By default, arm enables NEON for simd optimization")
    set(ARCH_ARM TRUE)
else ()
    set(ARCH_UNKNOWN TRUE)
endif ()

# Define options for SIMD instruction sets
option(USE_SIMD "Enable SIMD instructions" ON)
option(USE_FLAT_HASH_MAP "Enable FlatHashMap" ON)

# These options are only relevant if SIMD is enabled and the architecture is x86
if ((NOT USE_AVX2 AND (USE_SIMD AND ARCH_X86)) OR USE_SSE4_2)
    # Enable SSE4.2 instructions by default
    set(USE_SSE4_2 ON)
    set(USE_AVX2 OFF)
elseif (USE_AVX2 AND (USE_SIMD AND ARCH_X86))
    # Enable AVX2 instructions
    set(USE_SSE4_2 OFF)
else ()
    # Disable SIMD options for non-x86 architectures or if SIMD is disabled
    set(USE_SSE4_2 OFF)
    set(USE_AVX2 OFF)
endif ()

# Enforce mutual exclusivity between SSE4.2 and AVX2
if (USE_SSE4_2 AND USE_AVX2)
    message(FATAL_ERROR "Cannot enable both SSE4.2 and AVX2. Please choose only one")
endif ()

# Initialize a list to hold the SIMD flags
set(SIMD_FLAGS "")

add_library(ejson ${PROJECT_SOURCE_DIR}/src/ejson/jobject.cc ${PROJECT_SOURCE_DIR}/src/ejson/parser.cc ${PROJECT_SOURCE_DIR}/src/ejson/base64.cc)

# Apply SSE4.2 flag if enabled
if (USE_SSE4_2)
    list(APPEND SIMD_FLAGS "-msse4.2")
    target_compile_definitions(ejson PUBLIC USE_SSE4_2=ON)
endif ()

# Apply AVX2 flag if enabled
if (USE_AVX2)
    list(APPEND SIMD_FLAGS "-mavx2")
    target_compile_definitions(ejson PUBLIC USE_AVX2=ON)
endif ()

if (USE_FLAT_HASH_MAP)
    message(STATUS "Use FlatHashMap")
    target_compile_definitions(ejson PUBLIC USE_FLAT_HASH_MAP=ON)
    add_subdirectory(${PROJECT_SOURCE_DIR}/third-party/parallel-hashmap)
    target_link_libraries(ejson PUBLIC phmap)
endif ()

if (USE_SIMD)
    message(STATUS "Use SIMD Flags:${SIMD_FLAGS}")
    target_compile_options(ejson PRIVATE ${SIMD_FLAGS})
endif ()

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

