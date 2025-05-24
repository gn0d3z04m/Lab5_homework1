set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
cmake_minimum_required(VERSION 3.4)
option(COVERAGE "Check coverage" ON)

project(lab5)

include_directories("banking/" "tests/" "third-party/")

add_subdirectory(banking)
add_subdirectory(tests)
add_subdirectory(third-party/gtest)

if(BUILD_TESTS)
  enable_testing()
  add_test(NAME check_acc COMMAND check_acc)
  add_test(NAME check_tr COMMAND check_tr)
endif()

if (COVERAGE)
	target_compile_options(check PRIVATE --coverage)
	target_link_libraries(check --coverage)
endif()
