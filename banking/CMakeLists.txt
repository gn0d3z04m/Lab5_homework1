set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(banking)

add_library(account Account.cpp Account.h)
add_library(transaction Transaction.cpp Transaction.h)
set_target_properties(account PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(transaction PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(transaction account)
