# dsa-cpp — CMake build (alongside build.sh, same C++17, no dependencies)
#
#   cmake -S . -B build        # configure (out-of-source, keeps the tree clean)
#   cmake --build build        # builds demo + run-tests
#   ./build/demo               # printed demo of every structure
#   ctest --test-dir build     # or ./build/run-tests — the 200-check suite
#
# Options:
#   -DDSA_CPP_BUILD_TESTS=OFF  # skip the test target
#   -DCMAKE_BUILD_TYPE=Release # default: no optimisation flags beyond -O2
#                              # via CMAKE_CXX_FLAGS if you want them
cmake_minimum_required(VERSION 3.16)
project(dsa-cpp CXX)

# The structures are header-only C++17 templates; the only compiled
# units are the demo and the assertion-based test main.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Same warnings as build.sh, so both build paths stay honest.
add_compile_options(-Wall -Wextra)

# Demo: exercises every structure and prints the results.
add_executable(demo src/main.cpp)
target_include_directories(demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Tests: 200 assertion checks over every structure, wired into CTest
# so `ctest --test-dir build` reports pass/fail like any CMake project.
option(DSA_CPP_BUILD_TESTS "Build the assertion-based test suite" ON)
if(DSA_CPP_BUILD_TESTS)
  add_executable(run-tests test/test_main.cpp)
  target_include_directories(run-tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

  include(CTest)
  add_test(NAME tests COMMAND run-tests)
endif()
