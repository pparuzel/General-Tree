project(GeneralTests)

add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
add_executable(GeneralTests basic/basic_empty_test.cpp insert/insert_small_test.cpp)

target_link_libraries(GeneralTests gtest gtest_main)