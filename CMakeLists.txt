cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

set(PROJECT_NAME polynomial)
project(${PROJECT_NAME})

set(PROJ_LIBRARY "${PROJECT_NAME}")
set(PROJ_TESTS   "test_${PROJECT_NAME}")

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include" gtest)

add_subdirectory(src)
add_subdirectory(samples)
add_subdirectory(gtest)
add_subdirectory(test)