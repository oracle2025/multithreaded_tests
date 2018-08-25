cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(googletest)

include_directories(src)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(tests src/books.cpp test/books_test.cpp src/countdownlatch.cpp src/runsinthreads.cpp)
target_link_libraries(tests gtest gtest_main)

