# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/acking-liu/CLionProjects/ejson4cpp/build/_deps/doctest-src"
  "/Users/acking-liu/CLionProjects/ejson4cpp/build/_deps/doctest-build"
  "/Users/acking-liu/CLionProjects/ejson4cpp/build/_deps/doctest-subbuild/doctest-populate-prefix"
  "/Users/acking-liu/CLionProjects/ejson4cpp/build/_deps/doctest-subbuild/doctest-populate-prefix/tmp"
  "/Users/acking-liu/CLionProjects/ejson4cpp/build/_deps/doctest-subbuild/doctest-populate-prefix/src/doctest-populate-stamp"
  "/Users/acking-liu/CLionProjects/ejson4cpp/build/_deps/doctest-subbuild/doctest-populate-prefix/src"
  "/Users/acking-liu/CLionProjects/ejson4cpp/build/_deps/doctest-subbuild/doctest-populate-prefix/src/doctest-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/acking-liu/CLionProjects/ejson4cpp/build/_deps/doctest-subbuild/doctest-populate-prefix/src/doctest-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/acking-liu/CLionProjects/ejson4cpp/build/_deps/doctest-subbuild/doctest-populate-prefix/src/doctest-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
