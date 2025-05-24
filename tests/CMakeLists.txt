set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(tests)

add_executable(check_acc test_account.cpp)
add_executable(check_tr test_transaction.cpp)
target_link_libraries(check_acc gtest gtest_main gmock_main)
target_link_libraries(check_tr gtest gtest_main gmock_main)
target_link_libraries(check_acc account)
target_link_libraries(check_tr transaction)
