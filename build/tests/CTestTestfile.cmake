# CMake generated Testfile for 
# Source directory: /Users/acking-liu/CLionProjects/ejson4cpp/tests
# Build directory: /Users/acking-liu/CLionProjects/ejson4cpp/build/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(ejson-unittest "/Users/acking-liu/CLionProjects/ejson4cpp/build/tests/unittest")
set_tests_properties(ejson-unittest PROPERTIES  _BACKTRACE_TRIPLES "/Users/acking-liu/CLionProjects/ejson4cpp/tests/CMakeLists.txt;29;add_test;/Users/acking-liu/CLionProjects/ejson4cpp/tests/CMakeLists.txt;0;")
subdirs("../_deps/doctest-build")
subdirs("../_deps/nanobench-build")
