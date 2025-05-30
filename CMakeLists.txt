set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
cmake_minimum_required(VERSION 3.4)

project(lab5)
include(CTest)

include_directories("banking/" "tests/" "third-party/")

add_subdirectory(banking)
add_subdirectory(tests)
add_subdirectory(third-party/gtest)

target_compile_options(check_acc PRIVATE --coverage)
target_link_options(check_acc PRIVATE --coverage)
target_compile_options(check_tr PRIVATE --coverage)
target_link_options(check_tr PRIVATE --coverage)

target_include_directories(banking PUBLIC banking)

if(BUILD_TESTS)
  enable_testing()
  add_test(NAME check_acc COMMAND check_acc)
  add_test(NAME check_tr COMMAND check_tr)
endif()
