# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.26.4)

# We name the project and the target for the ExternalProject_Add() call
# to something that will highlight to the user what we are working on if
# something goes wrong and an error message is produced.

project(nanobench-populate NONE)


# Pass through things we've already detected in the main project to avoid
# paying the cost of redetecting them again in ExternalProject_Add()
set(GIT_EXECUTABLE [==[/usr/local/bin/git]==])
set(GIT_VERSION_STRING [==[2.42.0]==])
set_property(GLOBAL PROPERTY _CMAKE_FindGit_GIT_EXECUTABLE_VERSION
  [==[/usr/local/bin/git;2.42.0]==]
)


include(ExternalProject)
ExternalProject_Add(nanobench-populate
                     "UPDATE_DISCONNECTED" "False" "GIT_REPOSITORY" "https://gitee.com/acking-you/nanobench.git" "GIT_TAG" "v4.1.0" "GIT_SHALLOW" "TRUE"
                    SOURCE_DIR          "/Users/acking-liu/CLionProjects/ejson4cpp/build/_deps/nanobench-src"
                    BINARY_DIR          "/Users/acking-liu/CLionProjects/ejson4cpp/build/_deps/nanobench-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
                    USES_TERMINAL_PATCH     YES
)


